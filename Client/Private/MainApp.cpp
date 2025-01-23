#include "MainApp.h"
#include "GameInstance.h"
#include "Level_Loading.h"

CMainApp::CMainApp()
	: m_pGameInstance { GameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	/* 엔진을 사용할 준비를 하자. */
	ENGINE_DESC			EngineDesc{};

	EngineDesc.hWnd = g_hWnd;
	EngineDesc.isWindowed = true;
	EngineDesc.iWidth_VP = g_iWinSizeX;
	EngineDesc.iHeight_VP = g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL; 

	//GameInstance를 통해서 엔진을 만들었고, Level을 열 예정
	if (FAILED(Start_Level(LEVEL_LOGO)))
		return E_FAIL;

	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
#ifdef _DEBUG
	m_fTimeAcc += fTimeDelta;
#endif

	return;
}

HRESULT CMainApp::Render()
{

#ifdef _DEBUG

	++m_iNumDraw;

	if (m_fTimeAcc >= 1.f)
	{
		wsprintf(m_szFPS, TEXT("FPS : %d"), m_iNumDraw);

		m_iNumDraw = 0;
		m_fTimeAcc = 0.f;
	}	

	//SetWindowText(g_hWnd, m_szFPS);
	
#endif

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();
	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eLevelID)
{
	//if (FAILED(Level_Loading::Create(m_pDevice, m_pContext, LEVEL_LOADING)))
	//	return E_FAIL;

	// 위 방식처럼 호출해서 만들 수는 있으나, 위 방식으로 만들게 되면 GameInstance를 만든 의미도 없어지고
	// 데이터 관리 및 흐름이 망가지게 됨
	// 우리는 GameInstance를 통해서 Engine에서 다루는 것들을 호출해서 사용하기로 함

	// 왜 GameInstance를 호출해서 하는가?

	// Level은 우리가 Engine에서 만들고 이를 Client에서 dllimport를 하여 다루는데, 이 레벨을 다루는
	// Level_Manager가 Engine에서 만들어져 있고, 이를 GameInstance가 갖고 있기 때문

	//if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, Level_Loading::Create(m_pDevice, m_pContext, LEVEL_LOADING))))
	//	return E_FAIL;

	// Start_Level을 호출할 때 받아온 레벨값으로 실행할 것임

	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, Level_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;

	return S_OK;
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : CMainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMainApp::Free()
{	
	__super::Free();

	/*내 멤버를 정리한다. */

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);

}
