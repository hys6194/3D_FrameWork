#include "Body_FallenDog.h"

#include "GameInstance.h"

CBody_FallenDog::CBody_FallenDog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CBody_Monster{ pDevice, pContext }
{
}

CBody_FallenDog::CBody_FallenDog(const CBody_FallenDog& Prototype)
	: CBody_Monster{ Prototype }
{
}

HRESULT CBody_FallenDog::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CBody_FallenDog::Initialize(void* pArg)
{
	NULL_CHECK_RETURN(pArg, E_FAIL);

	FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);
	FAILED_CHECK_RETURN(Ready_SocketMatrices(), E_FAIL);

	return S_OK;
}

void CBody_FallenDog::Priority_Update(_float fTimeDelta)
{
}

void CBody_FallenDog::Update(_float fTimeDelta)
{
	__super::Update(fTimeDelta);
}

void CBody_FallenDog::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CBody_FallenDog::Render()
{
	if (FAILED(Bind_SR()))
		return E_FAIL;

	__super::Render();

	return S_OK;

}

HRESULT CBody_FallenDog::Ready_Component()
{
	_uint iNum = m_pGameInstance->Draw_RandomNum(2);
	if(1 == iNum)
	{
		FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_DOG,
			reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);
	}
	else
	{
		FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_MODEL_DOG_LAVA,
			reinterpret_cast<CComponent**>(&m_pModelCom), TEXT("Com_Model")), E_FAIL);
	}

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_ANIM,
		reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

	return S_OK;
}

HRESULT CBody_FallenDog::Ready_SocketMatrices()
{
	// 이건 Monster를 상속받는 녀석들에게 선언을 하고 실행하자

	m_mapSocketmat.emplace(SOCKET_DOG_LEFT_HAND,  m_pModelCom->Get_BoneMatrix("Bone_Hand_L_FD"));
	m_mapSocketmat.emplace(SOCKET_DOG_RIGHT_HAND, m_pModelCom->Get_BoneMatrix("Bone_Hand_R_FD"));

	return S_OK;
}

HRESULT CBody_FallenDog::Bind_SR()
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

CBody_FallenDog* CBody_FallenDog::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CBody_FallenDog* pInstance = new CBody_FallenDog(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Body_Ghoul");
		Safe_Release(pInstance);
	}
	return pInstance;

}

CGameObject* CBody_FallenDog::Clone(void* pArg)
{
	CBody_FallenDog* pInstance = new CBody_FallenDog(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Body_Ghoul");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CBody_FallenDog::Free()
{
	__super::Free();
}
