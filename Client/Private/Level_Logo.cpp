#include "Level_Logo.h"

Level_Logo::Level_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Level{ pDevice, pContext }
{
}

HRESULT Level_Logo::Initialize()
{
	return E_NOTIMPL;
}

void Level_Logo::Update(_float fTimeDelta)
{
}

HRESULT Level_Logo::Render()
{
	return E_NOTIMPL;
}

Level_Logo* Level_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

void Level_Logo::Free()
{
}
