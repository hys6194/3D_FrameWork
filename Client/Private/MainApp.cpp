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

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
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

	SetWindowText(g_hWnd, m_szFPS);
	
#endif

	m_pGameInstance->Clear_BackBuffer_View(_float4(0.f, 0.f, 1.f, 1.f));
	m_pGameInstance->Clear_DepthStencil_View();
	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Start_Level()
{
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, Level_Loading::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pGameInstance->Open_Level(LEVEL_LOADING, Level_Loading::Create(m_pDevice, m_pContext), E_FAIL);

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

	
}
