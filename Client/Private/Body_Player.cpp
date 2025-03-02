#include "Body_Player.h"
#include "GameInstance.h"

#include "Player.h"

Body_Player::Body_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : PartObject{ pDevice, pContext }
{
}

Body_Player::Body_Player(const Body_Player& Prototype)
    : PartObject{ Prototype }
{
}

const _float4x4* Body_Player::Get_f4SocketMatrix(const _wstring& strSocketName)
{
    auto iter = m_mapSocketmat.find(strSocketName);
    if (iter == m_mapSocketmat.end())
        return nullptr;


    return iter->second;
}

HRESULT Body_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT Body_Player::Initialize(void* pArg)
{
    NULL_CHECK_RETURN(pArg, E_FAIL);

    BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);
    m_pTargetState = pDesc->pTargetState;

    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);
    FAILED_CHECK_RETURN(Ready_SocketMatrices(), E_FAIL);

    m_pModelCom->Set_AnimationIndex(3, true);

    return S_OK;
}

void Body_Player::Priority_Update(_float fTimeDelta)
{
}

void Body_Player::Update(_float fTimeDelta)
{

    if (*m_pTargetState & Player::STATE_IDLE)
        m_pModelCom->Set_AnimationIndex(4, true);
    if (*m_pTargetState & Player::STATE_WALK)
        m_pModelCom->Set_AnimationIndex(3, true);

    m_pModelCom->Play_Animation(fTimeDelta);

    //파츠들의 매트릭스를 부모 매트릭스에 곱하여 고정시킨다
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentMatrix));


}

void Body_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(Renderer::RENDER_NONBLEND, this);
}

HRESULT Body_Player::Render()
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

HRESULT Body_Player::Ready_Components()
{
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Test"),
        reinterpret_cast<Component**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
        reinterpret_cast<Component**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

    return S_OK;
}
 
HRESULT Body_Player::Ready_SocketMatrices()
{
    NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

    // 특정 뼈의 매트릭스를 가져와야 함
    m_mapSocketmat.emplace(TEXT("Socket_Weapon"), m_pModelCom->Get_BoneMatrix("SWORD"));
    m_mapSocketmat.emplace(TEXT("Socket_Shadow"), m_pModelCom->Get_BoneMatrix("PlayerShadow"));

    return S_OK;
}

HRESULT Body_Player::Bind_SR()
{
    // Combined된 월드행렬을 반환해야 한다 -> 부모의 행렬만 가져오게 되면 로컬 정점에서만 적용된 행렬을 가져오기 때문
    // 
    // FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pParentMatrix), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, PipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, PipeLine::D3DTS_PROJ), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4)), E_FAIL);

    const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
    NULL_CHECK_RETURN(pLightDesc, E_FAIL);

    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDir",      &pLightDesc->vDirection, sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDiffuse",  &pLightDesc->vDiffuse,   sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightAmbient",  &pLightDesc->vAmbient,   sizeof(_float4)), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular,  sizeof(_float4)), E_FAIL);
   

    return S_OK;
}

Body_Player* Body_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Body_Player* pInstance = new Body_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Body_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

GameObject* Body_Player::Clone(void* pArg)
{
    Body_Player* pInstance = new Body_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Body_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Body_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
