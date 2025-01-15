#include "Level_GamePlay.h"

Level_GamePlay::Level_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Level{ pDevice, pContext }
{
}

HRESULT Level_GamePlay::Initialize()
{
	return S_OK;
}

void Level_GamePlay::Update(_float fTimeDelta)
{
}

HRESULT Level_GamePlay::Render()
{
	return S_OK;
}

Level_GamePlay* Level_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	return nullptr;
}

void Level_GamePlay::Free()
{
}
