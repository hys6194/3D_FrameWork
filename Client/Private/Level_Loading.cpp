#include "Level_Loading.h"

Level_Loading::Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Level{ pDevice, pContext }
{
}

HRESULT Level_Loading::Initialize()
{
	return E_NOTIMPL;
}

void Level_Loading::Update(_float fTimeDelta)
{
}

HRESULT Level_Loading::Render()
{
	return E_NOTIMPL;
}

Level_Loading* Level_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

void Level_Loading::Free()
{
}
