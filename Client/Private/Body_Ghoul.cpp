#include "Body_Ghoul.h"

#include "GameInstance.h"

CBody_Ghoul::CBody_Ghoul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBody_Monster{ pDevice, pContext }
{
}

CBody_Ghoul::CBody_Ghoul(const CBody_Ghoul& Prototype)
	: CBody_Monster{ Prototype }
{
}

HRESULT CBody_Ghoul::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Ghoul::Initialize(void* pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);

	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_SocketMatrices(), E_FAIL);

	return S_OK;
}

void CBody_Ghoul::Priority_Update(_float fTimeDelta)
{
}

void CBody_Ghoul::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	//XMStoreFloat4x4(&m_CombinedWorldMatrix,
	//	XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentMatrix));
}

void CBody_Ghoul::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	//m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBody_Ghoul::Render()
{
	if (FAILED(Bind_SR()))
		return E_FAIL;

	__super::Render();

	return S_OK;

}

HRESULT CBody_Ghoul::Ready_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_GHOUL,
		reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_ANIM,
		reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

	return S_OK;
}

HRESULT CBody_Ghoul::Ready_SocketMatrices()
{
	// 이건 Monster를 상속받는 녀석들에게 선언을 하고 실행하자
	// Bone_SW_Hand_L
	// Bone_SW_Hand_R
	// Bone_SW_Finger_Middle1_R
	// Bone_SW_Finger_Middle1_L

	m_mapSocketmat.emplace(SOCKET_GHOUL_LEFT_HAND,  m_pModelCom->Get_BoneMatrix("Bone_SW_Finger_Middle1_L"));
	m_mapSocketmat.emplace(SOCKET_GHOUL_RIGHT_HAND, m_pModelCom->Get_BoneMatrix("Bone_SW_Finger_Middle1_R"));

	return S_OK;
}

HRESULT CBody_Ghoul::Bind_SR()
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

CBody_Ghoul* CBody_Ghoul::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Ghoul* pInstance = new CBody_Ghoul(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Body_Ghoul");
		Safe_Release(pInstance);
	}
	return pInstance;

}

CGameObject* CBody_Ghoul::Clone(void* pArg)
{
	CBody_Ghoul* pInstance = new CBody_Ghoul(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Body_Ghoul");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBody_Ghoul::Free()
{
	__super::Free();
}
