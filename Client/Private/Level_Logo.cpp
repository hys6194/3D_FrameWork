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
	if (FAILED(Ready_Layer_Logo1(TEXT("Layer_Logo1"))))
		return E_FAIL;

	return S_OK;
}

void Level_Logo::Update(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("현재 레벨 : 로고 레벨"));

	m_fTotalTime += m_pGameInstance->Get_TimeDelta(TIME60);

	if (/*GetKeyState(VK_SPACE) & 0x8000 &&*/ 0.1f < m_fTotalTime)
	{
		if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, Level_Loading::Create(m_pDevice, m_pContext, LEVEL_GAMEPLAY))))
			return;
	}
}

HRESULT Level_Logo::Render()
{
	return S_OK;
}

HRESULT Level_Logo::Ready_Layer_Logo1(const _tchar* pLayerTag)
{
	UIObject::UIOBJECT_DESC			 UIDesc = {};

	// 선생님은 가로, 세로 1인 길이로 그리려고 0.5로 준 것이다
	UIDesc.fX = g_iWinSizeX  >> 1;
	UIDesc.fY = g_iWinSizeY >> 1;
	UIDesc.fSizeX = 800;
	UIDesc.fSizeY = 200;
	lstrcpy(UIDesc.szGameObjectTag, TEXT("GAMEOBJECT_UI_BACKGROUND"));
	UIDesc.fSpeedPerSec = 10.f;
	UIDesc.fRotationPerSec = XMConvertToRadians(180.f);

	if(FAILED(m_pGameInstance->Add_GameObject(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"), LEVEL_LOGO, pLayerTag, &UIDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT Level_Logo::Add_Logo(const _tchar* pLayerTag)
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


//// 동영상 재생 코드
//m_hVideo = MCIWndCreate(g_hWnd,		// 부모의 윈도우 핸들
//	nullptr,	// mci 윈도우를 사용하는 인스턴스 핸들
//	WS_CHILD | WS_VISIBLE | MCIWNDF_NOPLAYBAR,
//	L"../Video/THQ_LogoBasic.wmv");	// 재생할 파일 경로 전달
//
//// MoveWindow : 재생할 동영상의 크기를 설정
//
//MoveWindow(m_hVideo, 0, 0, WINCX, WINCY, FALSE);
//
//MCIWndPlay(m_hVideo);
// 
// 
//// 동영상 끝났을 때 || 스페이스 바 눌렀을 때 전환
//if (MCIWndGetLength(m_hVideo) <= MCIWndGetPosition(m_hVideo) ||
//	CKeyMgr::Get_Instance()->Key_Down(VK_SPACE))
//{
//	CSceneMgr::Get_Instance()->Scene_Change(SC_MENU);
//	return;
//}
