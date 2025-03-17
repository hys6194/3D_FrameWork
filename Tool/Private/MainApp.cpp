#include "MainApp.h"
#include "GameInstance.h"

#include "Level_Loading.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/ImGuizmo.h"

CMainApp::CMainApp()
	: m_pGameInstance { CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CMainApp::Initialize()
{
	// 엔진 사용준비
	ENGINE_DESC				EngineDesc{};

	EngineDesc.hInstance	= g_hInstance;
	EngineDesc.hWnd			= g_hWnd;
	EngineDesc.isWindowed	= true;
	EngineDesc.iWidth_VP	= g_iWinSizeX;
	EngineDesc.iHeight_VP	= g_iWinSizeY;
	EngineDesc.iNumLevels	= LEVEL_END;

	// 엔진 초기화
	if (FAILED(m_pGameInstance->Initialize_Engine(EngineDesc, &m_pDevice, &m_pContext)))
		return E_FAIL;

	// 레벨 시작
	if (FAILED(Start_Level(LEVEL_TOOL)))
		return E_FAIL;


	return S_OK;
}

void CMainApp::Update(_float fTimeDelta)
{
	if (nullptr == m_pGameInstance)
		return;


	m_pGameInstance->Update_Engine(fTimeDelta);


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

	m_pGameInstance->Draw_Engine();

	m_pGameInstance->Present();

	return S_OK;
}

HRESULT CMainApp::Start_Level(LEVEL eLevelID)
{
	// 어떤 레벨을 Create할 지 알아야 하기 때문에 LEVEL enum을 인자로 받아와서 호출한다.
	if (FAILED(m_pGameInstance->Open_Level(LEVEL_LOADING, Level_Loading::Create(m_pDevice, m_pContext, eLevelID))))
		return E_FAIL;

	return S_OK;
}

HRESULT CMainApp::Ready_Fonts()
{
	/*MakeSpriteFont "넥슨lv1고딕 Bold" /FontSize:16 /FastPack /CharacterRegion:0x0020-0x00FF /CharacterRegion:0x3131-0x3163 /CharacterRegion:0xAC00-0xD800 /DefaultCharacter:0xAC00 148ex.spritefont */

	if (FAILED(m_pGameInstance->Add_Font(TEXT("Font_Default"), TEXT("../Bin/Resources/Fonts/149ex.spritefont"))))
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

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	m_pGameInstance->Release_Engine();
	Safe_Release(m_pGameInstance);
}
