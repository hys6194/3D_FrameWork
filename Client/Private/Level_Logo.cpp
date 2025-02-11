#include "Level_Logo.h"

#include "Level_Loading.h"
#include "GameInstance.h"
#include "BackGround.h"

Level_Logo::Level_Logo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Level{ pDevice , pContext }
{
}

HRESULT Level_Logo::Initialize()
{
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_BackGround"))))
		return E_FAIL;

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

HRESULT Level_Logo::Ready_Layer_BackGround(const _tchar* pLayerTag)
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

	if(FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"), LEVEL_LOGO, pLayerTag, &UIDesc)))
		return E_FAIL;

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
