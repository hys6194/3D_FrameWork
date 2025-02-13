#include "Level_Menu.h"

#include "Level_Loading.h"
#include "GameInstance.h"
#include "BackGround.h"

Level_Menu::Level_Menu(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Level{ pDevice , pContext }
{
}

HRESULT Level_Menu::Initialize()
{
	if (FAILED(Ready_Layer_LogoPlayer(TEXT("Layer_BackGround"))))
		return E_FAIL;

	return S_OK;
}

void Level_Menu::Update(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("현재 레벨 : 메뉴 레벨"));

	if (GetKeyState(VK_SPACE) & 0x8000)
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, Level_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}

	return;
}

HRESULT Level_Menu::Render()
{
	return S_OK;
}

HRESULT Level_Menu::Ready_Layer_LogoPlayer(const _tchar* pLayerTag)
{
	UIObject::UIOBJECT_DESC			 UIDesc = {};

	// 선생님은 가로, 세로 1인 길이로 그리려고 0.5로 준 것이다
	UIDesc.fX = g_iWinSizeX * 0.5f;
	UIDesc.fY = g_iWinSizeY * 0.5f;
	UIDesc.fSizeX = g_iWinSizeX;
	UIDesc.fSizeY = g_iWinSizeY;
	lstrcpy(UIDesc.szGameObjectTag, TEXT("GAMEOBJECT_UI_BACKGROUND"));
	UIDesc.fSpeedPerSec = 10.f;
	UIDesc.fRotationPerSec = XMConvertToRadians(180.f);

	if(FAILED(m_pGameInstance->Add_GameObject(LEVEL_MENU, TEXT("Prototype_GameObject_BackGround"), LEVEL_MENU, pLayerTag, &UIDesc)))
		return E_FAIL;

	return S_OK;
}

Level_Menu* Level_Menu::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Level_Menu* pInstance = new Level_Menu(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Level_Menu");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Level_Menu::Free()
{
	__super::Free();
}
