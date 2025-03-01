#include "Weapon.h"
#include "GameInstance.h"

#include "Player.h"

Weapon::Weapon(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : PartObject{ pDevice, pContext }
{
}

Weapon::Weapon(const Weapon& Prototype)
    : PartObject{ Prototype }
{
}

HRESULT Weapon::Initialize_Prototype()
{
    return S_OK;
}

HRESULT Weapon::Initialize(void* pArg)
{
    NULL_CHECK_RETURN(pArg, E_FAIL);

    WEAPON_DESC* pDesc = static_cast<WEAPON_DESC*>(pArg);
    m_pTargetState = pDesc->pTargetState;
    m_pSocketMatrix = pDesc->pSocketMatrix;

    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);

    m_pTransformCom->SetUp_Scaled(0.1f, 0.1f, 0.1f);
    m_pTransformCom->Rotation(XMVectorSet(0.f, 1.f, 0.f, 0.f), XMConvertToRadians(90.f));
    m_pTransformCom->Set_State(Transform::STATE_POSITION, XMVectorSet(0.7f, 0.f, 0.f, 1.f));

    
    //m_pModelCom->Set_AnimationIndex(3);

    return S_OK;
}

void Weapon::Priority_Update(_float fTimeDelta)
{
}

void Weapon::Update(_float fTimeDelta)
{
    _matrix matSocket = XMLoadFloat4x4(m_pSocketMatrix);

    // 스케일 값에 영향을 주는 본이라면 그것을 없애주는 코드이다
    // 만약 이 코드가 없어도 잘 돌아간다면 원래 맞는 모델이라고 볼 수 있다
    for (size_t i = 0; i < 3; i++)
    {
        matSocket.r[i] = XMVector3Normalize(matSocket.r[i]);
    }

    // 파츠들의 매트릭스를 부모 매트릭스에 곱하여 고정시킨다
    // 그와 동시에, Socket 매트릭스를 같이 곱하여 월드 매트릭스 상에 binding 한다
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * 
        matSocket *
        XMLoadFloat4x4(m_pParentMatrix));


}

void Weapon::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(Renderer::RENDER_NONBLEND, this);

}

HRESULT Weapon::Render()
{
    if (FAILED(Bind_SR()))
        return E_FAIL;

    _uint		iNumMeshes = m_pModelCom->Get_NumMeshes();

    for (size_t i = 0; i < iNumMeshes; i++)
    {
        if (FAILED(m_pModelCom->Bind_Material(m_pShaderCom, "g_DiffuseTexture",
            aiTextureType_DIFFUSE, i, 0)))
            return E_FAIL;

        m_pModelCom->Bind_BoneMatrix(m_pShaderCom, "g_BoneMatrices", i);

        if (FAILED(m_pShaderCom->Begin(0)))
            return E_FAIL;

        if (FAILED(m_pModelCom->Render(i)))
            return E_FAIL;
    }

    return S_OK;
}

HRESULT Weapon::Ready_Components()
{
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
        reinterpret_cast<Component**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
        reinterpret_cast<Component**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

    return S_OK;
}

HRESULT Weapon::Bind_SR()
{
    // Combined된 월드행렬을 반환해야 한다
    // 
    //FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pParentMatrix), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, PipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, PipeLine::D3DTS_PROJ), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4)), E_FAIL);

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
    NULL_CHECK_RETURN(pLightDesc, E_FAIL);

    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)), E_FAIL);
   

    return S_OK;
}

Weapon* Weapon::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Weapon* pInstance = new Weapon(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Weapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

GameObject* Weapon::Clone(void* pArg)
{
    Weapon* pInstance = new Weapon(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Weapon");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Weapon::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
