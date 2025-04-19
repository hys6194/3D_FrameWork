#include "Body_Player.h"
#include "GameInstance.h"

#include "Player.h"

CBody_Player::CBody_Player(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CPartObject{ pDevice, pContext }
{
}

CBody_Player::CBody_Player(const CBody_Player& Prototype)
    : CPartObject{ Prototype }
{
}

const _float4x4* CBody_Player::Get_f4SocketMatrix(const _wstring& strSocketName)
{
    auto iter = m_mapSocketmat.find(strSocketName);
    if (iter == m_mapSocketmat.end())
        return nullptr;

    return iter->second;
}

HRESULT CBody_Player::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CBody_Player::Initialize(void* pArg)
{
    NULL_CHECK_RETURN(pArg, E_FAIL);

    BODY_PLAYER_DESC* pDesc = static_cast<BODY_PLAYER_DESC*>(pArg);
    m_pTargetState = pDesc->pTargetState;

    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);
    FAILED_CHECK_RETURN(Ready_SocketMatrices(), E_FAIL);

    return S_OK;
}

void CBody_Player::Priority_Update(_float fTimeDelta)
{
}

void CBody_Player::Update(_float fTimeDelta)
{
    //파츠들의 매트릭스를 부모 매트릭스에 곱하여 고정시킨다
    XMStoreFloat4x4(&m_CombinedWorldMatrix,
        XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentMatrix));

}

void CBody_Player::Late_Update(_float fTimeDelta)
{
    m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
} 

HRESULT CBody_Player::Render()
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

HRESULT CBody_Player::Ready_Components()
{
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_STRIFE,
        reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);

     FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_ANIM,
        reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

    return S_OK;
}
 
HRESULT CBody_Player::Ready_SocketMatrices()
{
    NULL_CHECK_RETURN(m_pShaderCom, E_FAIL);

    // 특정 뼈의 매트릭스를 가져와야 함
    // 특정 뼈의 이름은 모델의 뼈와 완벽히 일치해야 한다
    //m_mapSocketmat.emplace(TEXT("Socket_Weapon"), m_pModelCom->Get_BoneMatrix("SWORD"));
    //m_mapSocketmat.emplace(TEXT("Socket_Shadow"), m_pModelCom->Get_BoneMatrix("PlayerShadow"));

    m_mapSocketmat.emplace(SOCKET_LEFT_HAND,        m_pModelCom->Get_BoneMatrix("Bone_Strife_Hand_L"));
    m_mapSocketmat.emplace(SOCKET_RIGHT_HAND,       m_pModelCom->Get_BoneMatrix("Bone_Strife_Hand_R"));
    m_mapSocketmat.emplace(SOCKET_HOLSTER_LEFT,     m_pModelCom->Get_BoneMatrix("Bone_Strife_Holster_L"));
    m_mapSocketmat.emplace(SOCKET_HOLSTER_RIGHT,    m_pModelCom->Get_BoneMatrix("Bone_Strife_Holster_R"));

    //    m_mapSocketmat.emplace(SOCKET_LEFT_HAND,        m_pModelCom->Get_BoneMatrix("Bone_Strife_Fing_Thumb3_L_end_end_end_end"));
    //    m_mapSocketmat.emplace(SOCKET_RIGHT_HAND,       m_pModelCom->Get_BoneMatrix("Bone_Strife_Fing_Thumb3_R_end_end_end_end"));

    return S_OK;
}

HRESULT CBody_Player::Bind_SR()
{
    // Combined된 월드행렬을 반환해야 한다 -> 부모의 행렬만 가져오게 되면 로컬 정점에서만 적용된 행렬을 가져오기 때문
    // 
    // FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", m_pParentMatrix), E_FAIL);
    FAILED_CHECK_RETURN(m_pShaderCom->Bind_Matrix("g_WorldMatrix", &m_CombinedWorldMatrix), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, CPipeLine::D3DTS_VIEW), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, CPipeLine::D3DTS_PROJ), E_FAIL);

    return S_OK;
}

CBody_Player* CBody_Player::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CBody_Player* pInstance = new CBody_Player(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Body_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CBody_Player::Clone(void* pArg)
{
    CBody_Player* pInstance = new CBody_Player(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Body_Player");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CBody_Player::Free()
{
    __super::Free();

    Safe_Release(m_pShaderCom);
    Safe_Release(m_pModelCom);
}
