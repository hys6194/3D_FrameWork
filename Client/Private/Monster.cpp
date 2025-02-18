#include "Monster.h"

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
	return E_NOTIMPL;
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
	return E_NOTIMPL;
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
