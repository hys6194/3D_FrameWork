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
	return E_NOTIMPL;
}

HRESULT Monster::Initialize(void* pArg)
{
	GameObject::GAMEOBJECT_DESC			Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Monster"));
	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = 0.f;

	FAILED_CHECK_RETURN(__super::Initialize(&Desc), E_FAIL);
	FAILED_CHECK_RETURN(Ready_Component(), E_FAIL);	

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
}

HRESULT Monster::Render()
{
	return S_OK;
}

HRESULT Monster::Ready_Component()
{
	return S_OK;
}

HRESULT Monster::Bind_SR()
{
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_SR(m_pShaderCom, "g_WorldMatrix"));
	FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_ShaderResource(m_pShaderCom, "g_ViewMatrix", PipeLine::D3DTS_VIEW), E_FAIL);
	//FAILED_CHECK_RETURN(m_pTransformCom->Bind_SR(m_pShaderCom, "g_WorldMatrix"));
	//FAILED_CHECK_RETURN(m_pTransformCom->Bind_SR(m_pShaderCom, "g_WorldMatrix"));

	return S_OK;
}

Monster* Monster::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

GameObject* Monster::Clone(void* pArg)
{
	return nullptr;
}

void Monster::Free()
{
}
