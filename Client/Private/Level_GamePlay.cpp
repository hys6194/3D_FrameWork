#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"
#include "TP_Camera.h"

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

    return S_OK;
}

void CLevel_GamePlay::Update(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("현재 레벨 : 게임플레이 레벨"));
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
	TPCam_Desc.fFov = XMConvertToRadians(65.f);
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

	//for (size_t i = 0; i < 10; i++)
	//{
	//	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Monster"),
	//		LEVEL_GAMEPLAY, pLayerTag)))
	//		return E_FAIL;
	//}


	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Layer_Player(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_PLAYER,
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT CLevel_GamePlay::Ready_Lights()
{
	LIGHT_DESC		LightDesc{};

	LightDesc.eType = LIGHT_DESC::TYPE_DIRECTIONAL;
	LightDesc.vDirection = _float4(1.f, -1.f, 1.f, 0.f);
	LightDesc.vDiffuse = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vAmbient = _float4(1.f, 1.f, 1.f, 1.f);
	LightDesc.vSpecular = _float4(1.f, 1.f, 1.f, 1.f);

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
