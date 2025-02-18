#include "Loader.h"
#include "GameInstance.h"

#include "BackGround.h"
#include "Terrain.h"
#include "Monster.h"

Loader::Loader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ GameInstance::GetInstance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	Loader* pLoader = static_cast<Loader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT Loader::Initialize(LEVEL eNextLevelID)
{
	// 멀티 쓰레드를 사용할 시 값이 제대로 초기화가 안되는 현상이 있음
	m_eNextLevelID = eNextLevelID;

	InitializeCriticalSection(&m_CriticalSection);
	
	// 아래 방식으로 쓰레드를 추가
	m_hThread = (HANDLE)_beginthreadex(nullptr, 0, LoadingMain, this, 0, nullptr);
	if (0 == m_hThread)
		return E_FAIL;

	return S_OK;
}

HRESULT Loader::Loading()
{
	// 쓰레드 열기
	EnterCriticalSection(&m_CriticalSection);

	// 로딩 레벨함수를 HRESULT타입으로 만들었으므로 값을 받아오기 위한 지역변수
	HRESULT		hr = { };

	// Initialize에서 받아온 레벨값에 따라서 Loding 레벨을 선택한다
	switch (m_eNextLevelID)
	{
	case LEVEL_MENU:
		hr = Loading_Menu();
		break;
	case LEVEL_LOGO:
		hr = Loading_Logo();
		break;
	case LEVEL_GAMEPLAY:
		hr = Loading_GamePlay();
		break;
	}

	// 쓰레드 닫기
	LeaveCriticalSection(&m_CriticalSection);

	return hr;
}

void Loader::Show_LoadingState()
{
	SetWindowText(g_hWnd, m_szLoading);
}

HRESULT Loader::Loading_Logo()
{
	m_IsFin = false;

	/* 로고용 자원을 로드합니다.  */
	lstrcpy(m_szLoading, TEXT("텍스쳐를(을) 로딩중입니다."));

	/* Prototype_Component_Texture_BackGround */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
		Texture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("모델를(을) 로딩중입니다."));

	/* Prototype_Component_VIBuffer_Rect */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
		VIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("셰이더를(을) 로딩중입니다."));
	/* Prototype_Component_Shader_VtxPosTex */
	///* D3D11_INPUT_ELEMENT_DESC : 내 정점을 구성하는 멤버변수 하나의 정보를 표현하기위한 구조체. */
	D3D11_INPUT_ELEMENT_DESC        ElementDesc[2] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex"),
		Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), ElementDesc, 2))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("원형객체를(을) 로딩중입니다."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
		BackGround::Create(m_pDevice, m_pContext, LEVEL_LOGO))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩을 완료하였습니다."));

	m_IsFin = true;

	return S_OK;
}

HRESULT Loader::Loading_Menu()
{
	m_IsFin = false;

	lstrcpy(m_szLoading, TEXT("텍스쳐 로딩중."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_Texture_BackGround"),
		Texture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("모델 로딩중."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_VIBuffer_Rect"),
		VIBuffer_Rect::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("셰이더 로딩중."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_Shader_VtxPosTex"),
		Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::ElementDesc, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("원형객체 로딩중."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_GameObject_BackGround"),
		BackGround::Create(m_pDevice, m_pContext, LEVEL_MENU))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩 완료."));

	m_IsFin = true;

	return S_OK;
}

HRESULT Loader::Loading_GamePlay()
{
	m_IsFin = false;

	lstrcpy(m_szLoading, TEXT("텍스쳐 로딩중."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		Texture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.jpg")))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("모델 로딩중."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		VIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("셰이더 로딩중."));

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::ElementDesc, VTXNORTEX::iNumElements))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("원형객체 로딩중."));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		Terrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	lstrcpy(m_szLoading, TEXT("로딩 완료."));

	m_IsFin = true;

	return S_OK;
}

Loader* Loader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	Loader* pInstance = new Loader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : Loader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Loader::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pGameInstance);

	DeleteCriticalSection(&m_CriticalSection);
}
