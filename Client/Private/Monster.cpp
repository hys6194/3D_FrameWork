#include "Monster.h"

#include "GameInstance.h"

Monster::Monster(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:GameObject{ pDevice , pContext }
{
}

Monster::Monster(const Monster& Prototype)
	: GameObject{ Prototype }
{
}

HRESULT Monster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Monster::Initialize(void* pArg)
{
	GameObject::GAMEOBJECT_DESC			Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Monster"));
	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void Monster::Priority_Update(_float fTimeDelta)
{
}

void Monster::Update(_float fTimeDelta)
{
}

void Monster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(Renderer::RENDER_NONBLEND, this);
}

HRESULT Monster::Render()
{
	if (FAILED(Bind_SR()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pModelCom->Render()))
		return E_FAIL;

	return S_OK;
}

HRESULT Monster::Ready_Component()
{
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		reinterpret_cast<Component**>(&m_pModelCom), TEXT("Com_Model"))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
		reinterpret_cast<Component**>(&m_pShaderCom), TEXT("Com_Shader"))))
		return E_FAIL;

	return S_OK;
}

HRESULT Monster::Bind_SR()
{
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_SR(m_pShaderCom, "g_WorldMatrix"), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_ShaderResource(m_pShaderCom, "g_ViewMatrix", PipeLine::D3DTS_VIEW), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_ShaderResource(m_pShaderCom, "g_ProjMatrix", PipeLine::D3DTS_PROJ), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4)), E_FAIL);
	
	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	NULL_CHECK_RETURN(pLightDesc, E_FAIL);
	
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)), E_FAIL);

	//if (FAILED(m_pTransformCom->Bind_SR(m_pShaderCom, "g_WorldMatrix")))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Bind_VP_Transform_ShaderResource(m_pShaderCom, "g_ViewMatrix", PipeLine::D3DTS_VIEW)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Bind_VP_Transform_ShaderResource(m_pShaderCom, "g_ProjMatrix", PipeLine::D3DTS_PROJ)))
	//	return E_FAIL;
	//
	////if (FAILED(m_pTextureCom->Bind_ShaderResource(m_pShaderCom, "g_DiffuseTexture", 0)))
	////	return E_FAIL;	
	//
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
	//	return E_FAIL;
	//
	//const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	//if (nullptr == pLightDesc)
	//	return E_FAIL;
	//
	//
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4))))
	//	return E_FAIL;
	//if (FAILED(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4))))
	//	return E_FAIL;


	return S_OK;
}

Monster* Monster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Monster* pInstance = new Monster(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

GameObject* Monster::Clone(void* pArg)
{
	Monster* pInstance = new Monster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Monster::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pModelCom);
}
