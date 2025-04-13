#include "Body_Moloch.h"

#include "GameInstance.h"

CBody_Moloch::CBody_Moloch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBody_Monster{ pDevice, pContext }
{
}

CBody_Moloch::CBody_Moloch(const CBody_Moloch& Prototype)
	: CBody_Monster{ Prototype }
{
}

HRESULT CBody_Moloch::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_Moloch::Initialize(void* pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);

	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_SocketMatrices(), E_FAIL);

	return S_OK;
}

void CBody_Moloch::Priority_Update(_float fTimeDelta)
{
}

void CBody_Moloch::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);

	//XMStoreFloat4x4(&m_CombinedWorldMatrix,
	//	XMLoadFloat4x4(m_pTransformCom->Get_WorldMatrix_Ptr()) * XMLoadFloat4x4(m_pParentMatrix));
}

void CBody_Moloch::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
	//m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CBody_Moloch::Render()
{
	if (FAILED(Bind_SR()))
		return E_FAIL;

	__super::Render();

	return S_OK;

}

HRESULT CBody_Moloch::Ready_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH,
		reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_ANIM,
		reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

	return S_OK;
}

HRESULT CBody_Moloch::Ready_SocketMatrices()
{
	m_mapSocketmat.emplace(SOCKET_MOLOCH_RIGHT_HAND, m_pModelCom->Get_BoneMatrix("Bone_Moloch_Weapon_Blade"));
	// Bone_Moloch_Hand_R
	//Bone_Moloch_Weapon_Blade
	return S_OK;
}

HRESULT CBody_Moloch::Bind_SR()
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

CBody_Moloch* CBody_Moloch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_Moloch* pInstance = new CBody_Moloch(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Body_Ghoul");
		Safe_Release(pInstance);
	}
	return pInstance;

}

CGameObject* CBody_Moloch::Clone(void* pArg)
{
	CBody_Moloch* pInstance = new CBody_Moloch(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Body_Ghoul");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBody_Moloch::Free()
{
	__super::Free();

	Safe_Release(m_pModelCom);
	Safe_Release(m_pShaderCom);

}
