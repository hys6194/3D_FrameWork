#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "TP_Camera.h"
#include "Map_Object.h"

#include "Crystal.h"

CLevel_GamePlay::CLevel_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CLevel { pDevice , pContext }
{
}

HRESULT CLevel_GamePlay::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Terrain"))))
		return E_FAIL;
	
	if (FAILED(Ready_Layer_Monster(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Effect(TEXT("Layer_Effect"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(TEXT("Layer_Objcet"))))
		return E_FAIL;

    return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("현재 레벨 : 게임플레이 레벨"));

	if (m_pGameInstance->Key_Down(DIK_F1))
		m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_GHOUL,
			LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

	if (m_pGameInstance->Key_Down(DIK_F2))
		m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_DOG,
			LEVEL_GAMEPLAY, TEXT("Layer_Monster"));
	
	if (m_pGameInstance->Key_Down(DIK_F8))
		m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_MOLOCH,
			LEVEL_GAMEPLAY, TEXT("Layer_Monster"));

}

HRESULT CLevel_GamePlay::Render()
{
    return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_TERRAIN,
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_SKY,
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	//CCamera_Free::CAMERA_FREE_DESC   FreeCam_Desc{};
	//
	//FreeCam_Desc.vEye = _float3(0.f, 20.f, -20.f);
	//FreeCam_Desc.vAt = _float3(0.f, 0.f, 0.f);
	//FreeCam_Desc.fFov = XMConvertToRadians(60.f);
	//FreeCam_Desc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	//FreeCam_Desc.fNear = 0.1f;
	//FreeCam_Desc.fFar = 300.f;
	//FreeCam_Desc.fMouseSensor = 0.05f;
	//lstrcpy(FreeCam_Desc.szGameObjectTag, TEXT("GameObject_Camera_Free"));
	//FreeCam_Desc.fSpeedPerSec = 10.f;
	//FreeCam_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	//
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_CAM_FREE,
	//	LEVEL_GAMEPLAY, pLayerTag, &FreeCam_Desc)))
	//	return E_FAIL;

	CTP_Camera::TP_CAMERA_DESC   TPCam_Desc{};
	
	TPCam_Desc.vEye = _float3(0.f, 10.f, -10.f);
	TPCam_Desc.vAt = _float3(0.f, 0.f, 0.f);
	TPCam_Desc.fFov = XMConvertToRadians(35.f);
	TPCam_Desc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	TPCam_Desc.fNear = 0.1f;
	TPCam_Desc.fFar = 300.f;
	TPCam_Desc.fMouseSensor = 0.05f;
	lstrcpy(TPCam_Desc.szGameObjectTag, TEXT("GameObject_TP_Camera"));
	TPCam_Desc.fSpeedPerSec = 10.f;
	TPCam_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_CAM_3RD,
		LEVEL_GAMEPLAY, pLayerTag, &TPCam_Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Monster(const _tchar* pLayerTag)
{
	//FAILED_CHECK_RETURN(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_MONSTER,
	//	LEVEL_GAMEPLAY, pLayerTag), E_FAIL);

	// 나중에 여기에서 Index를 추가하는 방식으로 구분을 하던가 해
	for (size_t i = 0; i < 2; i++)
	{
		//FAILED_CHECK_RETURN(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_GHOUL,
		//	LEVEL_GAMEPLAY, pLayerTag), E_FAIL);
	}

	//FAILED_CHECK_RETURN(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_MOLOCH,
	//	LEVEL_GAMEPLAY, pLayerTag), E_FAIL);


	// 테스트 몬스터
	// 
	FAILED_CHECK_RETURN(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_TEST_MONSTER,
		LEVEL_GAMEPLAY, pLayerTag), E_FAIL);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_PLAYER,
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Effect(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Snow"),
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;


	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Explosion"),
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;


	return S_OK;

}

HRESULT CLevel_GamePlay::Ready_Layer_Object(const _tchar* pLayerTag)
{
	_ulong          dwByte = {};
	CMap_Object::MAPOBJ_DESC Desc{};
	HANDLE          hFile = CreateFile(TEXT("../../Client/Bin/DataFiles/MapObjectData.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return E_FAIL;

	_uint iMapObjCount;
	ReadFile(hFile, &iMapObjCount, sizeof(_uint), &dwByte, nullptr);

	for (size_t i = 0; i < iMapObjCount; i++)
	{
		// 모델 타입
		MODELTYPE eModelType;
		ReadFile(hFile, &eModelType, sizeof(eModelType), &dwByte, nullptr);

		// 모델 이름
		size_t  iModelTagLen;
		ReadFile(hFile, &iModelTagLen, sizeof(iModelTagLen), &dwByte, nullptr);

		wstring strModelTag(iModelTagLen, L'\0');
		ReadFile(hFile, &strModelTag[0], iModelTagLen * sizeof(wchar_t), &dwByte, nullptr);

		// 오브젝트 이름
		size_t  iObjectTagLen;
		ReadFile(hFile, &iObjectTagLen, sizeof(iObjectTagLen), &dwByte, nullptr);

		wstring strObjectTag(iObjectTagLen, L'\0');
		ReadFile(hFile, &strObjectTag[0], iObjectTagLen * sizeof(wchar_t), &dwByte, nullptr);

		_float4x4 matWorld;
		ReadFile(hFile, &matWorld, sizeof(matWorld), &dwByte, nullptr);

		int a = 10;

		Desc.eType = eModelType;
		Desc.strModelTag = strModelTag;
		Desc.strObjectTag = strObjectTag;
		Desc.matWorld = matWorld;

		m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, Desc.strObjectTag, LEVEL_GAMEPLAY, TEXT("Layer_Objcet"), &Desc);
	}

	CloseHandle(hFile);

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC		LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	// 점 조명들
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(10.f, 5.f, 10.f, 1.f);
	LightDesc.fRange = 12.f;
	LightDesc.vDiffuse = _float4(1.f, 0.0f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	
	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;
	
	LightDesc.eType = LIGHT_DESC::TYPE_POINT;
	LightDesc.vPosition = _float4(20.f, 5.f, 10.f, 1.f);
	LightDesc.fRange = 12.f;
	LightDesc.vDiffuse = _float4(0.f, 1.f, 0.f, 1.f);
	LightDesc.vAmbient = _float4(0.4f, 0.4f, 0.4f, 1.f);
	LightDesc.vSpecular = LightDesc.vDiffuse;
	
	if (FAILED(m_pGameInstance->Add_Light(LightDesc)))
		return E_FAIL;

	return S_OK;
}

CLevel_GamePlay* CLevel_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CLevel_GamePlay* pInstance = new CLevel_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Level_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_GamePlay::Free()
{
	__super::Free();
}
