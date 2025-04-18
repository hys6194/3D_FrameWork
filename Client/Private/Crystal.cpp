#include "Crystal.h"
#include "GameInstance.h"

#include "Moloch.h"

CCrystal::CCrystal(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject { pDevice ,pContext }
{
}

CCrystal::CCrystal(const CCrystal& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CCrystal::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CCrystal::Initialize(void* pArg)
{
    CRYSTAL_DESC* pDesc = static_cast<CRYSTAL_DESC*>(pArg);

    m_fLifeTime = pDesc->fLifeTime;
    m_vCrystalPos = pDesc->vCrystalPos;
    m_vLook = pDesc->vLook;
    m_pOwner = pDesc->pOwner;
    m_strModelTag = pDesc->strModelTag;

    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);

    m_pTransformCom->Set_State(CTransform::STATE_POS, m_vCrystalPos);

    return S_OK;
}

void CCrystal::Priority_Update(_float fTimeDelta)
{
    //m_pColliderCom->Reset();
    
    // 크리스탈을 생성했을때 위치와 각도를 선정해야 함
    // 그러면 인자로 받아오면서 생성하는걸로
    // 우선 크리스탈을 생성하는 공격 패턴 클래스를 최대한 빠르게 생성한다

}

void CCrystal::Update(_float fTimeDelta)
{
    m_fTotalTime += m_pGameInstance->Get_TimeDelta(TIME60);

    if (m_fLifeTime < m_fTotalTime ||
        m_pColliderCom->Is_Coll())
    {
        m_pGameInstance->Secede_Update(m_pColliderCom->Get_Bounder());
        m_bDisappear = true;

        // 이 때 이펙트를 만드는 것도 방법이라고 생각하기는 함

    }

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

}

void CCrystal::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CCrystal::Render()
{
    if (m_bDisappear)
        return E_ABORT;

    if (FAILED(Bind_SR()))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture",
            aiTextureType_DIFFUSE, i, 0)))
            return E_FAIL;

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

#ifdef _DEBUG
    m_pColliderCom->Render();
#endif 

    return S_OK;
}

HRESULT CCrystal::Ready_Component()
{
    // 모델 어떻게 넘겨줄까 
    // Desc으로 받아서 할까
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, m_strModelTag,
        reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_MESH,
        reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL)
        ;
    CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereDesc{};
    SphereDesc.fRadius      = 2.f;
    SphereDesc.vCenter      = _float3(0.f, 0.f, 0.f);
    SphereDesc.strCollTag   = Get_Name();
    SphereDesc.iOption      = COLL_OPT::OP_IMPACT;
    SphereDesc.eType =      TYPE::TYPE_SPHERE;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_SPHERE,
        reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL, &SphereDesc)))
        return E_FAIL;

    m_pGameInstance->Regist_Update(m_pColliderCom->Get_Bounder());

    return S_OK;
}

HRESULT CCrystal::Bind_SR()
{
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pTransformCom->Get_WorldMatrix_Ptr()), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, CPipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, CPipeLine::D3DTS_PROJ), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4)), E_FAIL);

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
    NULL_CHECK_RETURN(pLightDesc, E_FAIL);

    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)), E_FAIL);
    return S_OK;
}

CCrystal* CCrystal::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CCrystal* pInstance = new CCrystal(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Bullet");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CCrystal::Clone(void* pArg)
{
    CCrystal* pInstance = new CCrystal(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Created : Bullet");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCrystal::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pColliderCom);
}
