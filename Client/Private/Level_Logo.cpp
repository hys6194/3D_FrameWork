#include "Level_Logo.h"

#include "Level_Loading.h"
#include "GameInstance.h"

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
	SetWindowText(g_hWnd, TEXT("현재 레벨 : 로고 레벨"));

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, Level_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}
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
	__super::Free();
}
