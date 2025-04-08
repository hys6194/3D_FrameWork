#include "Loader.h"
#include "GameInstance.h"


#include "Tool_FreeCam.h"
#include "Terrain.h"
#include "Map_Object.h"
#include "Navi_Cell.h"
//#include "Monster.h"



Loader::Loader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
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
	EnterCriticalSection(&m_CriticalSection);

	// 로딩 레벨함수를 HRESULT타입으로 만들었으므로 값을 받아오기 위한 지역변수
	HRESULT		hr = { };

	// Initialize에서 받아온 레벨값에 따라서 Loding 레벨을 선택한다
	switch (m_eNextLevelID)
	{
	case LEVEL_TOOL:
		hr = Loading_Tool();
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

HRESULT Loader::Loading_Tool()
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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_TEX_TERRAIN,
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Tile0.dds")))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_TEX_SKY,
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/SkyBox/Sky_%d.dds"), 4))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Mask */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_TEX_MASK,
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Mask.dds"), 1))))
		return E_FAIL;
	
	/* For.Prototype_Component_Texture_Brush */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_TEX_BRUSH,
		CTexture::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Brush.png"), 1))))
		return E_FAIL;
	

	return S_OK;
}

HRESULT Loader::Loading_Models()
{
	// 지형 출력
	/* For.Prototype_Component_VIBuffer_Terrain*/
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_COM_VI_TERRAIN,
		CVIBuffer_Flat_Terrain::Create(m_pDevice, m_pContext, TEXT("../Bin/Resources/Textures/Terrain/Height.bmp")))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_COM_VI_CUBE,
		CVIBuffer_Cube::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_COM_VI_GUIDE,
		CNavi_Cell::Create(m_pDevice, m_pContext))))
		return E_FAIL;
	_matrix		PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.001f, 0.001f, 0.001f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	PreTransformMatrix = XMMatrixScaling(0.002f, 0.002f, 0.002f) * XMMatrixRotationY(XMConvertToRadians(-90.f));

	//// 저장용


	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_WALL_TOP,
	// 	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
	// 		"../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Ruins/Wall_Top_A.fbx",
	// 		"../../Client/Bin/DataFiles/Nonanim/Wall_Top_A.bin", PreTransformMatrix))))
	// 	return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_ROCK1,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/DestRock1.bin"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_ROCK2,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/DestRock2.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_FLOOR1,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/TileFloor1.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_STAIR1,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Stair1.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_STAIR2,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Stair2.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_ROCK_TALL,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/DestRock_Tall.bin"))))
		return E_FAIL;

	// 불러오기 용
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_FENCE,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Fence_Lg_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_DAM_END,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Dam_EndCap_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_DAM_END_ALT,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Dam_EndCap_A_Alt.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_DAM_END_ALCOVE,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Dam_MasterAlcove_G.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_DAM_TRIM,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Dam_Trims_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_GROUND_DECAL,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Ground_Decal_LG_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_PILLAR_BASE,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Pillar_Base_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_BENT_DARK,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Pillar_Bent_Dark.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_BENT_LIGHT,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Pillar_Bent_Light.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_RUBBLESTONE,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Rubble_DesertStone.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_FORTRESS_WALL,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Fortress_Wall_01_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_PILLER_A_DARK,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Pillar_A_Dark.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_PILLER_A_LIGHT,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Pillar_A_Light.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_PILLER_B_DARK,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Pillar_B_Dark.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_PILLER_B_LIGHT,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Pillar_B_Light.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_RUIN_CNR_A,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Ruins_CNR_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_RUIN_FLOOR,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/RuinsFloor_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_WODDEN_CIRCLE,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Wooden_Circle_C.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_SPIKE_A,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/FortressSpike_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_SPIKE_E,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/FortressSpike_E.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_PILLAR_ALCOVE,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Pillar_Alcove_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_PILLAR_DECO_A,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Pillar_Base_Deco_A.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_RUIN_WALL_CNR_B,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Wall_Cnr_B.bin"))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_WALL_TOP,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/MapObjects/Wall_Top_A.bin"))))
		return E_FAIL;


	/* Colliders */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_COM_COLL,
		CCollider::Create(m_pDevice, m_pContext))))
		return E_FAIL;


	return S_OK;
}

HRESULT Loader::Loading_Shaders()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_SHADER_POS,
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxPosTex.hlsl"), VTXPOSTEX::ElementDesc, VTXPOSTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxNorTex */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_SHADER_NOR,
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxNorTex.hlsl"), VTXNORTEX::ElementDesc, VTXNORTEX::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_SHADER_MESH,
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxMesh.hlsl"), VTXMESH::ElementDesc, VTXMESH::iNumElements))))
		return E_FAIL;

	/* For.Prototype_Component_Shader_VtxAnimMesh */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_SHADER_ANIM,
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxAnimMesh.hlsl"), VTXANIMESH::ElementDesc, VTXANIMESH::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_SHADER_CUBE,
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_VtxCube.hlsl"), VTXCUBE::ElementDesc, VTXCUBE::iNumElements))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_SHADER_CELL,
		CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"),
			VTXPOS::ElementDesc, VTXPOS::iNumElements))))
		return E_FAIL;


	return S_OK;
}

HRESULT Loader::Loading_Prototype()
{

	// 이렇게 할 필요가 없는데 왜 이렇게 했지 이유가 있었는데
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_TERRAIN,
		Terrain::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_CAM_FREE,
		CTool_FreeCam::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_ROCK1,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_ROCK2,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_ROCK_TALL,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_FLOOR1,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_STAIR1,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_STAIR2,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_SPIKE_E,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_SPIKE_A,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_GROUND_DECAL,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_DAM_END,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_DAM_END_ALT,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_DAM_END_ALCOVE,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_FENCE,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_PILLAR_BASE,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_BENT_DARK,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_BENT_LIGHT,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_RUBBLESTONE,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_FORTRESS_WALL,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_PILLER_A_DARK,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_PILLER_A_LIGHT,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_PILLER_B_DARK,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_PILLER_B_LIGHT,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_RUIN_CNR_A,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_RUIN_FLOOR,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_WALL_TOP,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_DAM_TRIM,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_WODDEN_CIRCLE,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_PILLAR_ALCOVE,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_RUIN_WALL_CNR_B,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_PILLAR_DECO_A,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

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
