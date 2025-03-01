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

	m_pTransformCom->Set_State(Transform::STATE_POSITION,
		XMVectorSet(m_pGameInstance->Random(0, 10), 1.f,
			m_pGameInstance->Random(0, 10), 1.f));

	// 재생할 애니메이션의 상태를 지정한다
	// 추후에 애니메이션의 관리를 ENUM으로 관리할수 있지 않을까 싶음

	// 뼈와 애니메이션을 공유하고 있는 문제로, 
	m_pModelCom->Set_AnimationIndex(3, true);

	// rand 값으로 넣게 되었을 때, 제일 마지막에 출력이 되는 녀석을 기준으로 애니메이션이 출력이되면서 가속화되는 현상은 없어진다
	// 
	// m_pModelCom->Set_AnimationIndex(rand() % 10, true);
	// m_pModelCom->Set_AnimationIndex(rand() % 10, true);

	return S_OK;
}

void Monster::Priority_Update(_float fTimeDelta)
{
}

void Monster::Update(_float fTimeDelta)
{
	if (true == m_pModelCom->Play_Animation(fTimeDelta))
		int a = 10;
}


void Monster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(Renderer::RENDER_NONBLEND, this);
}

HRESULT Monster::Render()
{
	if (FAILED(Bind_SR()))
		return E_FAIL;

	_uint iNumMeshes = m_pModelCom->Get_NumMeshes();

	for (size_t i = 0; i < iNumMeshes; ++i)
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

HRESULT Monster::Ready_Component()
{
	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
		reinterpret_cast<Component**>(&m_pModelCom), TEXT("Com_Model")) ,E_FAIL);

	FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
		reinterpret_cast<Component**>(&m_pShaderCom), TEXT("Com_Shader")), E_FAIL);

	return S_OK;
}

HRESULT Monster::Bind_SR()
{
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_SR("g_WorldMatrix", m_pShaderCom), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, PipeLine::D3DTS_VIEW), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, PipeLine::D3DTS_PROJ), E_FAIL);
	
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4)), E_FAIL);
	
	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	NULL_CHECK_RETURN(pLightDesc, E_FAIL);
	
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)), E_FAIL);

	//if (FAILED(m_pTransformCom->Bind_SR("g_WorldMatrix", m_pShaderCom)))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom,  PipeLine::D3DTS_VIEW)))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, PipeLine::D3DTS_PROJ)))
	//	return E_FAIL;
	//
	////if (FAILED(m_pTextureCom->Bind_SR(m_pShaderCom, "g_DiffuseTexture", 0)))
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
		MSG_BOX("Failed To Created : Monster");
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
