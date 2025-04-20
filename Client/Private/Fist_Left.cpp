#include "Fist_Left.h"
#include "GameInstance.h"

#include "Monster.h"


CFist_Left::CFist_Left(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CFist_Left::CFist_Left(const CFist_Left& Prototype)
    : CPartObject{ Prototype }
{
}

HRESULT CFist_Left::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFist_Left::Initialize(void* pArg)
{
    NULL_CHECK_RETURN(pArg, E_FAIL);

    FIST_LEFT_DESC* pDesc = static_cast<FIST_LEFT_DESC*>(pArg);
    m_pTargetState = pDesc->pTargetState;
    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pHandMatrix = pDesc->pHandMatrix;
    m_pOwner = pDesc->pOwner;

    m_fCool = 0.f;

    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);

    m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSet(0.f, -0.5f, 0.f, 1.f));
 


    return S_OK;
}

void CFist_Left::Priority_Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSet(0.f, -0.5f, 0.f, 1.f));

    m_pColliderCom->Reset();
}

void CFist_Left::Update(_float fTimeDelta)
{
    _matrix		SocketMatrix = XMLoadFloat4x4(m_pHandMatrix);

    for (size_t i = 0; i < 3; i++)
        SocketMatrix.r[i] = XMVector3Normalize(SocketMatrix.r[i]);

    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) *
        SocketMatrix *
        XMLoadFloat4x4(m_pParentMatrix));

    m_pColliderCom->Update(XMLoadFloat4x4(&m_CombinedWorldMatrix));


}

void CFist_Left::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

    if (m_pOwner->Is_Dead())
        return;

#ifdef _DEBUG
    m_pGameInstance->Add_Renderer_DebugComponent(m_pColliderCom);
#endif
}

HRESULT CFist_Left::Render()
{
    if (m_pOwner->Is_Dead())
        return S_OK;

#ifdef _DEBUG
    m_pColliderCom->Render();
#endif 

    return S_OK;
}

HRESULT CFist_Left::Ready_Components()
{
    CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereDesc{};
    SphereDesc.fRadius = 0.5f;
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);
    SphereDesc.strCollTag = m_pOwner->Get_Name() + TEXT("_Fist_Left");
    SphereDesc.iOption = COLL_OPT::OP_IMPACT;
    SphereDesc.eType = TYPE::TYPE_SPHERE;


    // 이 방식이 훨 낫긴하다
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_SPHERE,
        reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL, &SphereDesc), E_FAIL);

    //m_pGameInstance->Regist_Update(m_pColliderCom->Get_Bounder());

    return S_OK;
}

HRESULT CFist_Left::Bind_SR()
{
    // Combined된 월드행렬을 반환해야 한다
    // 
    //FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pParentMatrix), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, CPipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, CPipeLine::D3DTS_PROJ), E_FAIL);
   

    return S_OK;
}

CFist_Left* CFist_Left::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFist_Left* pInstance = new CFist_Left(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Gun_Left");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFist_Left::Clone(void* pArg)
{
    CFist_Left* pInstance = new CFist_Left(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Gun_Left");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFist_Left::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
