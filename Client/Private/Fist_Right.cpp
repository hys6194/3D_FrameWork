#include "Fist_Right.h"
#include "GameInstance.h"

#include "Monster.h"


CFist_Right::CFist_Right(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CFist_Right::CFist_Right(const CFist_Right& Prototype)
    : CPartObject{ Prototype }
{
}

HRESULT CFist_Right::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CFist_Right::Initialize(void* pArg)
{
    NULL_CHECK_RETURN(pArg, E_FAIL);

    FIST_RIGHT_DESC* pDesc = static_cast<FIST_RIGHT_DESC*>(pArg);
    m_pTargetState = pDesc->pTargetState;
    m_pSocketMatrix = pDesc->pSocketMatrix;
    m_pHandMatrix = pDesc->pHandMatrix;
    m_pOwner = pDesc->pOwner;

    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);

    m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSet(0.f, -0.5f, 0.f, 1.f));

    return S_OK;
}

void CFist_Right::Priority_Update(_float fTimeDelta)
{
    m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSet(0.f, -0.5f, 0.f, 1.f));

    m_pColliderCom->Reset();
}

void CFist_Right::Update(_float fTimeDelta)
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

void CFist_Right::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

}

HRESULT CFist_Right::Render()
{
    if (m_pOwner->Is_Dead())
        return S_OK;

#ifdef _DEBUG
    m_pColliderCom->Render();
#endif 	

    return S_OK;
}

HRESULT CFist_Right::Ready_Components()
{
    CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereDesc{};
    SphereDesc.fRadius = 0.5f;
    SphereDesc.vCenter = _float3(0.f, SphereDesc.fRadius, 0.f);
    SphereDesc.strCollTag = m_pOwner->Get_Name() + TEXT("_Fist_Right ");
    SphereDesc.iOption = COLL_OPT::OP_IMPACT;
    SphereDesc.eType = TYPE::TYPE_SPHERE;

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_SPHERE,
        reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL, &SphereDesc), E_FAIL);

    return S_OK;
}

HRESULT CFist_Right::Bind_SR()
{
    
    return S_OK;
}

CFist_Right* CFist_Right::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CFist_Right* pInstance = new CFist_Right(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Gun_Right");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CFist_Right::Clone(void* pArg)
{
    CFist_Right* pInstance = new CFist_Right(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Gun_Right");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CFist_Right::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
