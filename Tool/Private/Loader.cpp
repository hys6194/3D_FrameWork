#include "Loader.h"
#include "GameInstance.h"


#include "Tool_FreeCam.h"
#include "Terrain.h"
#include "Map_Object.h"
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

	_matrix		PreTransformMatrix = XMMatrixIdentity();
	PreTransformMatrix = XMMatrixScaling(0.02f, 0.02f, 0.02f) * XMMatrixRotationY(XMConvertToRadians(-90.f));

	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_ROCK1,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Rock/DestRock1.fbx", PreTransformMatrix))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_ROCK2,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Rock/DestRock2.fbx", PreTransformMatrix))))
	//	return E_FAIL;
	//PreTransformMatrix = XMMatrixScaling(0.001f, 0.001f, 0.001f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_FLOOR1,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Floor/TileFloor1.fbx", PreTransformMatrix))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_STAIR1,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Stair/Stair1.fbx", PreTransformMatrix))))
	//	return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_STAIR2,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Stair/Stair2.fbx", PreTransformMatrix))))
	//	return E_FAIL;

	//// 저장용
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_ROCK1,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
	//		"../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Rock/DestRock1.fbx",
	//		"../../Client/Bin/DataFiles/Nonanim/DestRock1.bin",
	//		PreTransformMatrix))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_ROCK2,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
	//		"../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Rock/DestRock2.fbx",
	//		"../../Client/Bin/DataFiles/Nonanim/DestRock2.bin",
	//		PreTransformMatrix))))
	//	return E_FAIL;
	//
	//PreTransformMatrix = XMMatrixScaling(0.001f, 0.001f, 0.001f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_FLOOR1,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
	//		"../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Floor/TileFloor1.fbx",
	//		"../../Client/Bin/DataFiles/Nonanim/TileFloor1.bin",
	//		PreTransformMatrix))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_STAIR1,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
	//		"../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Stair/Stair1.fbx",
	//		"../../Client/Bin/DataFiles/Nonanim/Stair1.bin",
	//		PreTransformMatrix))))
	//	return E_FAIL;
	//
	//if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_STAIR2,
	//	CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM,
	//		"../../Client/Bin/Resources/Models/NonAnimModel/MapObject/Stair/Stair2.fbx",
	//		"../../Client/Bin/DataFiles/Nonanim/Stair2.bin",
	//		PreTransformMatrix))))
	//	return E_FAIL;

	// 불러오기
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_ROCK1,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/DestRock1.bin"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_ROCK2,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/DestRock2.bin"))))
		return E_FAIL;
	
	PreTransformMatrix = XMMatrixScaling(0.001f, 0.001f, 0.001f) * XMMatrixRotationY(XMConvertToRadians(-90.f));
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_FLOOR1,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/TileFloor1.bin"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_STAIR1,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/Stair1.bin"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_MODEL_STAIR2,
		CModel::Create(m_pDevice, m_pContext, "../../Client/Bin/DataFiles/Nonanim/Stair2.bin"))))
		return E_FAIL;
	
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_GAMEPLAY, PRO_MODEL_RGUN,
		CModel::Create(m_pDevice, m_pContext, MODELTYPE::TYPE_NONANIM, "../Bin/Resources/Models/AnimModel/Strife/Gun2.fbx", PreTransformMatrix))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_COM_COLL_AABB,
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_AABB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_OBB */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_COM_COLL_OBB,
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_OBB))))
		return E_FAIL;

	/* For.Prototype_Component_Collider_Sphere */
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_COM_COLL_SPHERE,
		CCollider::Create(m_pDevice, m_pContext, CCollider::TYPE_SPHERE))))
		return E_FAIL;


	return S_OK;
}

HRESULT Loader::Loading_Shaders()
{
	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_MENU, PRO_SHADER_POS,
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

	return S_OK;
}

HRESULT Loader::Loading_Prototype()
{

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

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_FLOOR1,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_FLOOR2,
		CMap_Object::Create(m_pDevice, m_pContext))))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_Prototype(LEVEL_TOOL, PRO_OBJ_FLOOR3,
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
