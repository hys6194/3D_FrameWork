#include "Level_Logo.h"

Level_Logo::Level_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Level{ pDevice , pContext }
{
}

HRESULT Level_Logo::Initialize()
{
	return S_OK;
}

void Level_Logo::Update(_float fTimeDelta)
{
}

HRESULT Level_Logo::Render()
{
	return S_OK;
}

Level_Logo* Level_Logo::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Level_Logo* pInstance = new Level_Logo(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Level_Logo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Level_Logo::Free()
{
}
