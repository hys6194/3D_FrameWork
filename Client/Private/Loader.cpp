#include "Loader.h"
#include "GameInstance.h"

#include "Camera_Free.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Monster.h"
#include "Player.h"
#include "Body_Player.h"
#include "Weapon.h"

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

	lstrcpy(m_szLoading, TEXT("텍스쳐를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Textures(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("모델를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Models(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("셰이더를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Shaders(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("원형객체를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Prototype(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("로딩 완료."));

	m_IsFin = true;

	return S_OK;
}

HRESULT Loader::Loading_Menu()
{
	m_IsFin = false;

	lstrcpy(m_szLoading, TEXT("텍스쳐를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Textures(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("모델를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Models(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("셰이더를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Shaders(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("원형객체를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Prototype(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("로딩 완료."));

	m_IsFin = true;

	return S_OK;
}

HRESULT Loader::Loading_GamePlay()
{
	m_IsFin = false;

	lstrcpy(m_szLoading, TEXT("텍스쳐를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Textures(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("모델를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Models(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("셰이더를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Shaders(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("원형객체를(을) 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Prototype(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("로딩 완료."));

	m_IsFin = true;

	return S_OK;
}

HRESULT Loader::Loading_Textures()
{
	switch (m_eNextLevelID)
	{
	case LEVEL_MENU:

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_Texture_BackGround"),
			Texture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
			return E_FAIL;

		break;
	case LEVEL_LOGO:

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
			Texture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Texture_Logo1"),
			Texture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo/UI_StudioLogo_THQNordic1.png")))))
			return E_FAIL;

		break;
	case LEVEL_GAMEPLAY:

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
			Texture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds")))))
			return E_FAIL;

		break;
	default:
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Loader::Loading_Models()
{	switch (m_eNextLevelID)
	{
	case LEVEL_MENU:

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_VIBuffer_Rect"),
			VIBuffer_Rect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		break;
	case LEVEL_LOGO:	
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_VIBuffer_Rect"),
			VIBuffer_Rect::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		break;
	case LEVEL_GAMEPLAY:
		// 지형 출력
		/* For.Prototype_Component_VIBuffer_Terrain*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
			VIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
			return E_FAIL;

		_matrix		PreTransformMatrix = XMMatrixIdentity();
		/* For.Prototype_Component_Model_Fiona */
		PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_Fiona"),
			Model::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM, "../Bin/Resources/Models/Fiona/Fiona.fbx", PreTransformMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_ForkLift */
		PreTransformMatrix = XMMatrixScaling(0.01f, 0.01f, 0.01f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Model_ForkLift"),
			Model::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../Bin/Resources/Models/ForkLift/ForkLift.fbx", PreTransformMatrix))))
			return E_FAIL;

		break;
	default:
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Loader::Loading_Shaders()
{
	switch (m_eNextLevelID)
	{
	case LEVEL_MENU:

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_Component_Shader_VtxPosTex"),
			Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::ElementDesc, VTXPOSTEX::iNumElements))))
			return E_FAIL;

		break;
	case LEVEL_LOGO:

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_Component_Shader_VtxPosTex"),
			Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::ElementDesc, VTXPOSTEX::iNumElements))))
			return E_FAIL;

		break;
	case LEVEL_GAMEPLAY:	

		/* For.Prototype_Component_Shader_VtxNorTex */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
			Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::ElementDesc, VTXNORTEX::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxMesh */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxMesh"),
			Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::ElementDesc, VTXMESH::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxAnimMesh */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxAnimMesh"),
			Shader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMESH::ElementDesc, VTXANIMESH::iNumElements))))
			return E_FAIL;

		break;
	default:
		return E_FAIL;
	}
	return S_OK;
}

HRESULT Loader::Loading_Prototype()
{
	switch (m_eNextLevelID)
	{
	case LEVEL_MENU:

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, TEXT("Prototype_GameObject_BackGround"),
			BackGround::Create(m_pDevice, m_pContext, LEVEL_MENU))))
			return E_FAIL;

		break;
	case LEVEL_LOGO:

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, TEXT("Prototype_GameObject_BackGround"),
			BackGround::Create(m_pDevice, m_pContext, LEVEL_LOGO))))
			return E_FAIL;

		break;
	case LEVEL_GAMEPLAY:

		/* Prototype_GameObject_Terrain */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
			Terrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* Prototype_GameObject_Monster */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster"),
			Monster::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player"),
			Player::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Player_Body"),
			Body_Player::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* Prototype_GameObject_Camera_Free */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
			Camera_Free::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Weapon"),
			Weapon::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		break;
	default:
		return E_FAIL;
	}

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
