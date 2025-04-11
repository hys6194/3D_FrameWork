#include "Bullet.h"
#include "GameInstance.h"


CBullet::CBullet(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject { pDevice ,pContext }
{
}

CBullet::CBullet(const CBullet& Prototype)
    : CGameObject{ Prototype }
{
}

HRESULT CBullet::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBullet::Initialize(void* pArg)
{
    BULLET_DESC* pDesc = static_cast<BULLET_DESC*>(pArg);

    m_matHand = pDesc->f4Hand;
    m_fBulletPos = pDesc->fBulletPos;
    m_fSpeed = pDesc->fSpeedPerSec;
    m_fLook = pDesc->fLook;
    m_iIndex = m_iIndex + 1;
    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Component(), E_FAIL)

    m_pTransformCom->Set_Matrix(&m_matHand);
    m_pTransformCom->SetUp_Scaled(3.f, 3.f, 3.f);

    return S_OK;
}

void CBullet::Priority_Update(_float fTimeDelta)
{
    //m_pColliderCom->Reset();

}

void CBullet::Update(_float fTimeDelta)
{
    m_fTotalTime += m_pGameInstance->Get_TimeDelta(TIME60);

    _vector vTest = m_pTransformCom->Get_State(CTransform::STATE_POS);
    vTest += XMLoadFloat4(&m_fLook) * m_fSpeed;

    //m_pTransformCom->Set_State(CTransform::STATE_POS, XMLoadFloat4(&m_fBulletPos));
    m_pTransformCom->Set_State(CTransform::STATE_POS, vTest);


    if (10.f < m_fTotalTime ||
        m_pColliderCom->Is_Coll())
    {
W        m_pGameInstance->Secede_Update(m_pColliderCom->Get_Bounder());
    }

    m_pColliderCom->Update(XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()));

}

void CBullet::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBullet::Render()
{
    if (m_pColliderCom->Is_Coll())
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

HRESULT CBullet::Ready_Component()
{

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_FORK,
        reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_MESH,
        reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL)
        ;
    CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereDesc{};
    SphereDesc.fRadius      = 0.2f;
    SphereDesc.vCenter      = _float3(0.f, 0.f, 0.f);
    //SphereDesc.strCollTag   = Get_Name() + std::to_wstring(m_iIndex);
    SphereDesc.strCollTag   = Get_Name();
    SphereDesc.iOption      = COLL_OPT::OP_IMPACT;
    SphereDesc.eType =      TYPE::TYPE_SPHERE;

    if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_SPHERE,
        reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL, &SphereDesc)))
        return E_FAIL;

    // 근데 좀 별로다 몸은 자동으로 등록하는데 총알이나 공격 부류는 내가 선언해야 등록되는거
    // 아닌데? 의도한대로 되긴했는데? 불편한거 아닌가?
    // 불편한거네 근데 이게 더 낫긴해
    m_pGameInstance->Regist_Update(m_pColliderCom->Get_Bounder());

    return S_OK;
}

HRESULT CBullet::Bind_SR()
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

CBullet* CBullet::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBullet* pInstance = new CBullet(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Bullet");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBullet::Clone(void* pArg)
{
    CBullet* pInstance = new CBullet(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Created : Bullet");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBullet::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
    Safe_Release(m_pColliderCom);
}
