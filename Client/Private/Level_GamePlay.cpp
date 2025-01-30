#include "Level_GamePlay.h"

Level_GamePlay::Level_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Level { pDevice , pContext }
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
	Level_GamePlay* pInstance = new Level_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Level_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Level_GamePlay::Free()
{
}
