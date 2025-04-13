#include "Moloch_Sword.h"
#include "GameInstance.h"

#include "Monster.h"


CMoloch_Sword::CMoloch_Sword(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CMoloch_Sword::CMoloch_Sword(const CMoloch_Sword& Prototype)
    : CPartObject{ Prototype }
{
}

HRESULT CMoloch_Sword::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMoloch_Sword::Initialize(void* pArg)
{
    NULL_CHECK_RETURN(pArg, E_FAIL);

    MOLOCH_SWORD_DESC* pDesc = static_cast<MOLOCH_SWORD_DESC*>(pArg);
    m_pTargetState = pDesc->pTargetState;
    m_pHandMatrix = pDesc->pHandMatrix;
    m_pOwner = pDesc->pOwner;

    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);

    //m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSet(0.f, -0.5f, 0.f, 1.f));

    return S_OK;
}

void CMoloch_Sword::Priority_Update(_float fTimeDelta)
{
    //m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSet(0.f, -0.5f, 0.f, 1.f));

    //m_pTransformCom->SetUp_Scaled(10.f, 10.f, 10.f);

    m_pColliderCom->Reset();
    //dynamic_cast<CBounding_OBB*>(m_pColliderCom->Get_Bounder())
    //    ->Get_Desc()->Extents =_float3(0.5f, 6.f, 1.8f);

}

void CMoloch_Sword::Update(_float fTimeDelta)
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

void CMoloch_Sword::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);

}

HRESULT CMoloch_Sword::Render()
{
    if (FAILED(Bind_SR()))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture",
            aiTextureType_DIFFUSE, i, 0)))
            return E_FAIL;

        //m_pModelCom->Bind_BoneMatrix(m_pShaderCom, "g_BoneMatrices", i);

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

HRESULT CMoloch_Sword::Ready_Components()
{
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH_SWORD,
        reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);
    
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_MESH,
       reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

    CBounding_OBB::BOUNDING_OBB_DESC		OBBDesc{};
    OBBDesc.vExtents = _float3(0.5f, 6.f, 1.8f);
    OBBDesc.vRotation = _float3(XMConvertToRadians(-90.f), 0.f, 0.f);
    OBBDesc.vCenter = _float3(0.f, 0.f, OBBDesc.vExtents.y);
    OBBDesc.strCollTag = m_pOwner->Get_Name() + TEXT("_Moloch_Sword ");
    OBBDesc.iOption = COLL_OPT::OP_IMPACT;
    OBBDesc.eType = TYPE::TYPE_OBB;

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_OBB,
        reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL, &OBBDesc), E_FAIL);

    return S_OK;
}

HRESULT CMoloch_Sword::Bind_SR()
{
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix), E_FAIL);
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

CMoloch_Sword* CMoloch_Sword::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMoloch_Sword* pInstance = new CMoloch_Sword(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Moloch_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMoloch_Sword::Clone(void* pArg)
{
    CMoloch_Sword* pInstance = new CMoloch_Sword(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Moloch_Sword");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMoloch_Sword::Free()
{
    __super::Free();

    Safe_Release(m_pColliderCom);
    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
