#include "Loader.h"
#include "GameInstance.h"

#include "Camera_Free.h"
#include "TP_Camera.h"
#include "BackGround.h"
#include "Terrain.h"
#include "Monster.h"
#include "Player.h"
#include "Body_Player.h"
#include "Weapon.h"
#include "Sky.h"
#include "Gun_Left.h"
#include "Gun_Right.h"

CLoader::CLoader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

unsigned int APIENTRY LoadingMain(void* pArg)
{
	CoInitializeEx(nullptr, 0);

	CLoader* pLoader = static_cast<CLoader*>(pArg);

	if (FAILED(pLoader->Loading()))
		return 1;

	return 0;
}

HRESULT CLoader::Initialize(LEVEL eNextLevelID)
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

HRESULT CLoader::Loading()
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

void CLoader::Show_LoadingState()
{
	SetWindowText(g_hWnd, m_szLoading);
}

HRESULT CLoader::Loading_Logo()
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

HRESULT CLoader::Loading_Menu()
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

HRESULT CLoader::Loading_GamePlay()
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

HRESULT CLoader::Loading_Textures()
{
	switch (m_eNextLevelID)
	{
	case LEVEL_MENU:
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, PRO_TEX_BACKGROUND,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
			return E_FAIL;
	}

		break;
	case LEVEL_LOGO:
	{

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, PRO_TEX_BACKGROUND,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.jpg"), 2))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, PRO_TEX_LOGO1,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo/UI_StudioLogo_THQNordic1.dds")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, PRO_TEX_LOGO2,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo/UI_StudioLogo_THQNordic2.png")))))
			return E_FAIL;
	}

		break;
	case LEVEL_GAMEPLAY:
	{

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_TEX_TERRAIN,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_TEX_SKY,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
			return E_FAIL;
	}

		break;
	default:
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoader::Loading_Models()
{	switch (m_eNextLevelID)
	{
	case LEVEL_MENU:
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, PRO_COM_VI_RECT,
			CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

		break;
	case LEVEL_LOGO:	
	{
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, PRO_COM_VI_RECT,
			CVIBuffer_Rect::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}

		break;
	case LEVEL_GAMEPLAY:
	{
		// 지형 출력
		/* For.Prototype_Component_VIBuffer_Terrain*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_VI_TERRAIN,
			CVIBuffer_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_VI_CUBE,
			CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		_matrix		PreTransformMatrix = XMMatrixIdentity();
		/* For.Prototype_Component_Model_Fiona */
		PreTransformMatrix = XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_FIONA,
			CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM, "../Bin/Resources/Models/AnimModel/Fiona/Fiona.fbx", PreTransformMatrix))))
			return E_FAIL;

		/* For.Prototype_Component_Model_ForkLift */
		PreTransformMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_FORK,
			CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../Bin/Resources/Models/NonAnimModel/ForkLift/ForkLift.fbx", PreTransformMatrix))))
			return E_FAIL;

		PreTransformMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_STRIFE,
			CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM, "../Bin/Resources/Models/AnimModel/Strife/animtest.fbx", PreTransformMatrix))))
			return E_FAIL;


		PreTransformMatrix = /*XMMatrixScaling(0.02f, 0.02f, 0.02f) **/ XMMatrixRotationZ(XMConvertToRadians(180.f));
		PreTransformMatrix *= /*XMMatrixScaling(0.02f, 0.02f, 0.02f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
		PreTransformMatrix *= /*XMMatrixScaling(0.02f, 0.02f, 0.02f) **/ XMMatrixRotationX(XMConvertToRadians(90.f));

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_LGUN,
			CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../Bin/Resources/Models/AnimModel/Strife/Gun1.fbx", PreTransformMatrix))))
			return E_FAIL;


		PreTransformMatrix = /*XMMatrixScaling(0.02f, 0.02f, 0.02f) **/ XMMatrixRotationZ(XMConvertToRadians(180.f));
		PreTransformMatrix *= /*XMMatrixScaling(0.02f, 0.02f, 0.02f) **/ XMMatrixRotationY(XMConvertToRadians(180.f));
		PreTransformMatrix *= /*XMMatrixScaling(0.02f, 0.02f, 0.02f) **/ XMMatrixRotationX(XMConvertToRadians(-90.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_RGUN,
			CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../Bin/Resources/Models/AnimModel/Strife/Gun2.fbx", PreTransformMatrix))))
			return E_FAIL;
	}


		break;
	default:
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoader::Loading_Shaders()
{
	switch (m_eNextLevelID)
	{
	case LEVEL_MENU:
	{

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, PRO_SHADER_POS,
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::ElementDesc, VTXPOSTEX::iNumElements))))
			return E_FAIL;
	}

		break;
	case LEVEL_LOGO:
	{

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, PRO_SHADER_POS,
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::ElementDesc, VTXPOSTEX::iNumElements))))
			return E_FAIL;
	}

		break;
	case LEVEL_GAMEPLAY:	
	{

		/* For.Prototype_Component_Shader_VtxNorTex */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_SHADER_NOR,
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::ElementDesc, VTXNORTEX::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxMesh */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_SHADER_MESH,
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::ElementDesc, VTXMESH::iNumElements))))
			return E_FAIL;

		/* For.Prototype_Component_Shader_VtxAnimMesh */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_SHADER_ANIM,
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMESH::ElementDesc, VTXANIMESH::iNumElements))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_SHADER_CUBE,
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::ElementDesc, VTXCUBE::iNumElements))))
			return E_FAIL;
	}

		break;
	default:
		return E_FAIL;
	}
	return S_OK;
}

HRESULT CLoader::Loading_Prototype()
{
	switch (m_eNextLevelID)
	{
	case LEVEL_MENU: 
	{

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, PRO_OBJ_BACK,
			CBackGround::Create(m_pDevice, m_pContext, LEVEL_MENU))))
			return E_FAIL;
	}

		break;
	case LEVEL_LOGO: 
		{

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, PRO_OBJ_BACK,
			CBackGround::Create(m_pDevice, m_pContext, LEVEL_LOGO))))
			return E_FAIL;
	}

		break;
	case LEVEL_GAMEPLAY:
	{

		/* Prototype_GameObject_Terrain */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_TERRAIN,
			CTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* Prototype_GameObject_Monster */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_MONSTER,
			CMonster::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_PLAYER,
			CPlayer::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_BODY,
			Body_Player::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* Prototype_GameObject_Camera_Free */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_CAM_FREE,
			CCamera_Free::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_CAM_3RD,
			CTP_Camera::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_WEAPON,
			Weapon::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_SKY,
			CSky::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_FSM,
			CFSM::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_L_GUN,
			CGun_Left::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_R_GUN,
			CGun_Right::Create(m_pDevice, m_pContext))))
			return E_FAIL;
	}


		break;
	default:
		return E_FAIL;
	}

	return S_OK;
}

CLoader* CLoader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	CLoader* pInstance = new CLoader(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : Loader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLoader::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

	Safe_Release(m_pGameInstance);

	DeleteCriticalSection(&m_CriticalSection);
}
