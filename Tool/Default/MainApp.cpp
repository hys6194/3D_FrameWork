#include "MainApp.h"
#include "GameInstance.h"

MainApp::MainApp()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT MainApp::Initialize()
{
	ENGINE_DESC			EngineDesc{};

	EngineDesc.hInstance		= g_hInstance;
	EngineDesc.hWnd				= g_hWnd;
	EngineDesc.isWindowed		= true;
	EngineDesc.iWidth_VP		= g_iWinSizeX;
	EngineDesc.iHeight_VP		= g_iWinSizeY;

	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	if (FAILED(Ready_ImGui()))
		return E_FAIL;

	return S_OK;
}

void MainApp::Update(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;


	m_pGameInstance->Update_Engine(fTimeDelta);



}

HRESULT MainApp::Render()
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


	m_pGameInstance->Clear_DepthStencil_View();

	m_pGameInstance->Draw_Engine();


	m_pGameInstance->Present();

	return S_OK;
}

HRESULT MainApp::Ready_ImGui()
{
	//if(FAILED(m_pGameInstance->))

	return S_OK;
}

MainApp* MainApp::Create()
{
	MainApp* pInstance = new MainApp();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : MainApp");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void MainApp::Free()
{
	__super::Free();

	/*내 멤버를 정리한다. */

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);
}
