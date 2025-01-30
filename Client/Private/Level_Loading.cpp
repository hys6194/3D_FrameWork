#include "Level_Loading.h"

Level_Loading::Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Level{ pDevice , pContext }
{
}

HRESULT Level_Loading::Initialize()
{
	return S_OK;
}

void Level_Loading::Update(_float fTimeDelta)
{
}

HRESULT Level_Loading::Render()
{
	return S_OK;
}

Level_Loading* Level_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Level_Loading* pInstance = new Level_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Level_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Level_Loading::Free()
{
}
