#include "Loader.h"
#include "GameInstance.h"


#include "Body_FallenDog.h"
#include "Test_Monster.h"
#include "Moloch_Sword.h"
#include "Body_Moloch.h"
#include "Body_Player.h"
#include "Camera_Free.h"
#include "BackGround.h"
#include "Map_Object.h"
#include "Body_Ghoul.h"
#include "Fist_Right.h"
#include "Fallen_Dog.h"
#include "Explosion.h"
#include "Fist_Left.h"
#include "TP_Camera.h"
#include "Gun_Right.h"
#include "Gun_Left.h"
#include "Crystal.h"
#include "Terrain.h"
#include "Attack.h"
#include "Bullet.h"
#include "Moloch.h"
#include "Player.h"
#include "Status.h"
#include "Weapon.h"
#include "Ghoul.h"
#include "Snow.h"
#include "Sky.h"


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

	lstrcpy(m_szLoading, TEXT("텍스쳐를 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Textures(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("모델을 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Models(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("셰이더를 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Shaders(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("원형객체를 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Prototype(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("콜라이더를 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Collider(), E_FAIL);

	lstrcpy(m_szLoading, TEXT("네비게이션을 로딩중입니다."));
	FAILED_CHECK_RETURN(Loading_Navigation(), E_FAIL);

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
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Default%d.dds"), 2))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, PRO_TEX_LOGO1,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo/UI_StudioLogo_THQNordic1.dds")))))
			return E_FAIL;

		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_LOGO, PRO_TEX_LOGO2,
		//	CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Logo/UI_StudioLogo_THQNordic2.png")))))
		//	return E_FAIL;
	}

		break;
	case LEVEL_GAMEPLAY:
	{

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile%d.dds"), 2))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_TEX_SKY,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
			return E_FAIL;


		/* For.Prototype_Component_Texture_Mask */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_TEX_MASK,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds"), 1))))
			return E_FAIL;

		/* For.Prototype_Component_Texture_Brush */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_TEX_BRUSH,
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
			CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Snow/Snow.png"), 1))))
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

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_FSM,
			CFSM::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_STATUS,
			CStatus::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_ATTACK,
			CAttack::Create(m_pDevice, m_pContext)))) 
			return E_FAIL;

		CVIBuffer_Particle::INSTANCE_PARTICLE_DESC		SnowDesc{};

		SnowDesc.iNumInstances = 3000;
		SnowDesc.vCenter = _float3(65.f, 10.f, 65.f);
		SnowDesc.vRange = _float3(129.f, 1.f, 129.f);
		SnowDesc.vSpeed = _float2(2.f, 10.f);
		SnowDesc.vLifeTime = _float2(1.f, 3.f);
		SnowDesc.vSize = _float2(0.1f, 0.3f);

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Particle_Snow"),
			CVIBuffer_Rect_Instancing::Create(m_pDevice, m_pContext, &SnowDesc))))
			return E_FAIL;

		CVIBuffer_Particle::INSTANCE_PARTICLE_DESC		ExploDesc{};

		ExploDesc.iNumInstances = 300;
		ExploDesc.vCenter = _float3(0.f, 0.f, 0.f);
		ExploDesc.vRange = _float3(0.2f, 0.2f, 0.2f);
		ExploDesc.vSpeed = _float2(2.f, 10.f);
		ExploDesc.vLifeTime = _float2(0.1f, 0.5f);
		ExploDesc.vSize = _float2(0.1f, 0.3f);
		ExploDesc.vPivot = _float3(0.f, -1.0f, 0.f);
		ExploDesc.isLoop = true;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Particle_Explosion"),
			CVIBuffer_Point_Instancing::Create(m_pDevice, m_pContext, &ExploDesc))))
			return E_FAIL;

		_matrix		PreTransformMatrix = XMMatrixIdentity();

		/////* For.Prototype_Component_Model_ForkLift */
		//PreTransformMatrix = XMMatrixScaling(0.2f, 0.2f, 0.2f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		// 
		// 새로운 애니메이션 모델 저장용
		//PreTransformMatrix = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_DOG,
		//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM,
		//		"../Bin/Resources/Models/AnimModel/FallenDog/FallenDog.fbx",
		//		"../Bin/DataFiles/anim/Creature/FallenDog.bin",
		//		PreTransformMatrix))))
		//	return E_FAIL;
		//
		//PreTransformMatrix = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_DOG_LAVA,
		//	 CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM,
		//		 "../Bin/Resources/Models/AnimModel/FallenDog/FallenDog_Lava.fbx",
		//		 "../Bin/DataFiles/anim/Creature/FallenDog_Lava.bin",
		//		 PreTransformMatrix))))
		//	 return E_FAIL;
		// 
		// PreTransformMatrix = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		// if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH,
		// 	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM,
		// 		"../Bin/Resources/Models/AnimModel/Moloch/Moloch.fbx",
		// 		"../Bin/DataFiles/anim/Creature/Moloch.bin",
		// 		PreTransformMatrix))))
		// 	return E_FAIL;
		// 
		//PreTransformMatrix = XMMatrixScaling(0.0002f, 0.0002f, 0.0002f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_FALLEN_GHOUL,
		//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM,
		//		"../Bin/Resources/Models/AnimModel/Ghoul/Fallen_Ghoul.fbx",
		//		"../Bin/DataFiles/anim/Creature/Fallen_Ghoul.bin",
		//		PreTransformMatrix))))
		//	return E_FAIL;
		// 
		// 
		// PreTransformMatrix = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		// if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_DOG_LAVA,
		// 	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM,
		// 		"../Bin/Resources/Models/AnimModel/FallenDog/FallenDog_Lava.fbx",
		// 		"../Bin/DataFiles/anim/Creature/FallenDog_Lava.bin",
		// 		PreTransformMatrix))))
		// 	return E_FAIL;
		// 
		//  새로운 논애니메이션 모델 저장용
		//PreTransformMatrix = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH_CRYSTAL_A,
		//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
		//		"../Bin/Resources/Models/NonAnimModel/Crystal/TremorCrystal_A.fbx",
		//		"../Bin/DataFiles/Nonanim/AttackObject/TremorCrystal_A.bin",
		//		PreTransformMatrix))))
		//	return E_FAIL;
		//
		//PreTransformMatrix = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH_CRYSTAL_B,
		//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
		//		"../Bin/Resources/Models/NonAnimModel/Crystal/TremorCrystal_B.fbx",
		//		"../Bin/DataFiles/Nonanim/AttackObject/TremorCrystal_B.bin",
		//		PreTransformMatrix))))
		//	return E_FAIL;

		//'PreTransformMatrix = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		//'if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH_SWORD,
		//'	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
		//'		"../Bin/Resources/Models/AnimModel/Moloch/Moloch_Sword.fbx",
		//'		"../Bin/DataFiles/Nonanim/PartObject/Moloch_Sword.bin",
		//'		PreTransformMatrix))))
		//'	return E_FAIL;
		//
		//// Strife 저장용
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_STRIFE,
		//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM,
		//		"../Bin/Resources/Models/AnimModel/Strife/animtest.fbx",
		//		"../Bin/DataFiles/anim/Heroes/Strife.bin",
		//		PreTransformMatrix))))
		//	return E_FAIL;
		//
		// Ghoul 저장용
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_GHOUL,
		//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_ANIM,
		//		"../Bin/Resources/Models/AnimModel/Ghoul/Ghoul.fbx",
		//		"../Bin/DataFiles/anim/Creature/Ghoul.bin",
		//		PreTransformMatrix))))
		//	return E_FAIL;
		//
		//PreTransformMatrix =  XMMatrixRotationZ(XMConvertToRadians(180.f));
		//PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.f));
		//PreTransformMatrix *= XMMatrixRotationX(XMConvertToRadians(90.f));
		//// Gun1 저장용
		//
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_LGUN,
		//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
		//		"../Bin/Resources/Models/AnimModel/Strife/Gun1.fbx",
		//		"../Bin/DataFiles/Nonanim/PartObject/Gun1.bin",
		//		PreTransformMatrix))))
		//	return E_FAIL;
		//// Gun2 저장용
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_RGUN,
		//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
				//		"../Bin/Resources/Models/AnimModel/Strife/Gun2.fbx",
				//		"../Bin/DataFiles/Nonanim/PartObject/Gun2.bin",
		//		PreTransformMatrix))))
		//	return E_FAIL;

		// ForkLift 불러오기


		PreTransformMatrix = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(180.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_FORK,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/PartObject/ForkLift.bin", PreTransformMatrix))))
			return E_FAIL;

		PreTransformMatrix = XMMatrixScaling(0.0018f, 0.0018f, 0.0018f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/anim/Creature/Moloch.bin", PreTransformMatrix))))
			return E_FAIL;

		// Strife 불러오기
		PreTransformMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_STRIFE,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/anim/Heroes/Strife.bin", PreTransformMatrix))))
			return E_FAIL;

		PreTransformMatrix = XMMatrixScaling(0.5f, 0.5f, 0.5f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH_CRYSTAL_A,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/AttackObject/TremorCrystal_A.bin", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH_CRYSTAL_B,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/AttackObject/TremorCrystal_B.bin", PreTransformMatrix))))
			return E_FAIL;

		PreTransformMatrix = XMMatrixRotationX(XMConvertToRadians(-90.f)) * XMMatrixRotationZ(XMConvertToRadians(-90.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_MOLOCH_SWORD,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/PartObject/Moloch_Sword.bin", PreTransformMatrix))))
			return E_FAIL;

		// Ghoul 불러오기
		PreTransformMatrix = XMMatrixScaling(0.0002f, 0.0002f, 0.0002f) * XMMatrixRotationY(XMConvertToRadians(-180.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_GHOUL,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/anim/Creature/Ghoul.bin", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_FALLEN_GHOUL,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/anim/Creature/Fallen_Ghoul.bin", PreTransformMatrix))))
			return E_FAIL;

		PreTransformMatrix = XMMatrixScaling(0.001f, 0.001f, 0.001f);
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_DOG,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/anim/Creature/FallenDog.bin", PreTransformMatrix))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_DOG_LAVA,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/anim/Creature/FallenDog_Lava.bin", PreTransformMatrix))))
			return E_FAIL;

		PreTransformMatrix =  XMMatrixRotationZ(XMConvertToRadians(180.f));
		PreTransformMatrix *= XMMatrixRotationY(XMConvertToRadians(180.f));
		PreTransformMatrix *= XMMatrixRotationX(XMConvertToRadians(90.f));
		// Gun1불러오기
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_LGUN,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/PartObject/Gun1.bin", PreTransformMatrix))))
			return E_FAIL;
		
		// Gun2 불러오기
		PreTransformMatrix *= XMMatrixRotationX(XMConvertToRadians(-90.f));
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_RGUN,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/PartObject/Gun2.bin", PreTransformMatrix))))
			return E_FAIL;

#pragma region MAP_OBJECT

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_ROCK1,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/DestRock1.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_ROCK2,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/DestRock2.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_FLOOR1,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/TileFloor1.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_STAIR1,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Stair1.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_STAIR2,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Stair2.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_ROCK_TALL,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/DestRock_Tall.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_SPIKE_A,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/FortressSpike_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_SPIKE_E,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/FortressSpike_E.bin"))))
			return E_FAIL;

		// 불러오기 용
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_FENCE,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Fence_Lg_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_DAM_END,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Dam_EndCap_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_DAM_END_ALT,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Dam_EndCap_A_Alt.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_DAM_END_ALCOVE,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Dam_MasterAlcove_G.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_DAM_TRIM,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Dam_Trims_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_GROUND_DECAL,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Ground_Decal_LG_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_PILLAR_BASE,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Pillar_Base_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_BENT_DARK,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Pillar_Bent_Dark.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_BENT_LIGHT,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Pillar_Bent_Light.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_RUBBLESTONE,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Rubble_DesertStone.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_FORTRESS_WALL,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Fortress_Wall_01_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_PILLER_A_DARK,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Pillar_A_Dark.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_PILLER_A_LIGHT,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Pillar_A_Light.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_PILLER_B_DARK,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Pillar_B_Dark.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_PILLER_B_LIGHT,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Pillar_B_Light.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_RUIN_CNR_A,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Ruins_CNR_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_RUIN_FLOOR,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/RuinsFloor_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_WODDEN_CIRCLE,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Wooden_Circle_C.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_PILLAR_ALCOVE,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Pillar_Alcove_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_PILLAR_DECO_A,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Pillar_Base_Deco_A.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_RUIN_WALL_CNR_B,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Wall_Cnr_B.bin"))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_WALL_TOP,
			CModel::Create(m_pDevice, m_pContext, "../Bin/DataFiles/Nonanim/MapObjects/Wall_Top_A.bin"))))
			return E_FAIL;

#pragma endregion



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

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxRectParticle"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxRectParticle.hlsl"), VTXPOSTEX_PARTICLE_INSTANCE::ElementDesc, VTXPOSTEX_PARTICLE_INSTANCE::iNumElements))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosParticle"),
			CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosParticle.hlsl"), VTXPOS_PARTICLE_INSTANCE::ElementDesc, VTXPOS_PARTICLE_INSTANCE::iNumElements))))
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
		/* Test_Monster_Anim*/
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_TEST_MONSTER,
			CTest_Monster::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* Prototype_GameObject_Terrain */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_TERRAIN,
			CTerrain::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		/* Prototype_GameObject_Monster */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_GHOUL,
			CGhoul::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_GHOUL_BODY,
			CBody_Ghoul::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_MOLOCH,
			CMoloch::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_MOLOCH_SWORD,
			CMoloch_Sword::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_MOLOCH_BODY,
			CBody_Moloch::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_DOG,
			CFallen_Dog::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_DOG_BODY,
			CBody_FallenDog::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_PLAYER,
			CPlayer::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_BODY,
			CBody_Player::Create(m_pDevice, m_pContext))))
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

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_L_GUN,
			CGun_Left::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_R_GUN,
			CGun_Right::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_L_FIST,
			CFist_Left::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_R_FIST,
			CFist_Right::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_BULLET,
			CBullet::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_CRYSTAL,
			CCrystal::Create(m_pDevice, m_pContext))))
			return E_FAIL;


#pragma region Map_Objects

		// 이건 진짜 잘못된 방향인 거 같기도 하다
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_ROCK1,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_ROCK2,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_ROCK_TALL,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_FLOOR1,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_STAIR1,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_STAIR2,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_SPIKE_A,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_SPIKE_E,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_GROUND_DECAL,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_DAM_END,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_DAM_END_ALT,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_DAM_END_ALCOVE,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_FENCE,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_PILLAR_BASE,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_BENT_DARK,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_BENT_LIGHT,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_RUBBLESTONE,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_FORTRESS_WALL,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_PILLER_A_DARK,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_PILLER_A_LIGHT,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_PILLER_B_DARK,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_PILLER_B_LIGHT,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_RUIN_CNR_A,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_RUIN_FLOOR,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_WALL_TOP,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_DAM_TRIM,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_WODDEN_CIRCLE,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_PILLAR_ALCOVE,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_RUIN_WALL_CNR_B,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_OBJ_PILLAR_DECO_A,
			CMap_Object::Create(m_pDevice, m_pContext))))
			return E_FAIL;

#pragma endregion

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Snow"),
			CSnow::Create(m_pDevice, m_pContext))))
			return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Explosion"),
			CExplosion::Create(m_pDevice, m_pContext))))
			return E_FAIL;

	}


		break;
	default:
		return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader::Loading_Collider()
{
	switch (m_eNextLevelID)
	{

	case LEVEL_GAMEPLAY:
		//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_COLL,
		//	CCollider::Create(m_pDevice, m_pContext))))
		//	return E_FAIL;

		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_COLL_AABB,
			CCollider::Create(m_pDevice, m_pContext, TYPE::TYPE_AABB))))
			return E_FAIL;
		
		/* For.Prototype_Component_Collider_OBB */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_COLL_OBB,
			CCollider::Create(m_pDevice, m_pContext, TYPE::TYPE_OBB))))
			return E_FAIL;
		
		/* For.Prototype_Component_Collider_Sphere */
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_COLL_SPHERE,
			CCollider::Create(m_pDevice, m_pContext, TYPE::TYPE_SPHERE))))
			return E_FAIL;
	}

	return S_OK;
}

HRESULT CLoader::Loading_Navigation()
{
	switch (m_eNextLevelID)
	{

	case LEVEL_GAMEPLAY:
		if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_COM_NAVI,
			CNavigation::Create(m_pDevice, m_pContext, TEXT("../Bin/DataFiles/Navigation.dat")))))
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
