#include "Level_GamePlay.h"
#include "GameInstance.h"
#include "Camera_Free.h"

Level_GamePlay::Level_GamePlay(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Level { pDevice , pContext }
{
}

HRESULT Level_GamePlay::Initialize()
{

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Terrain(TEXT("Layer_Terrain"))))
		return E_FAIL;

	//if (FAILED(Ready_Lights()))
	//	return E_FAIL;

    return S_OK;
}

void Level_GamePlay::Update(_float fTimeDelta)
{
	SetWindowText(g_hWnd, TEXT("현재 레벨 : 게임플레이 레벨"));
}

HRESULT Level_GamePlay::Render()
{
    return S_OK;
}

HRESULT Level_GamePlay::Ready_Layer_Terrain(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Terrain"),
		LEVEL_GAMEPLAY, pLayerTag)))
		return E_FAIL;

	return S_OK;
}

HRESULT Level_GamePlay::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	Camera_Free::CAMERA_FREE_DESC   Desc{};

	Desc.vEye = _float3(0.f, 10.f, -10.f);
	Desc.vAt = _float3(0.f, 0.f, 0.f);
	Desc.fFov = XMConvertToRadians(60.f);
	Desc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	Desc.fNear = 0.1f;
	Desc.fFar = 300.f;
	Desc.fMouseSensor = 0.05f;
	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Camera"));
	Desc.fSpeedPerSec = 10.f;
	Desc.fRotationPerSec = XMConvertToRadians(90.f);

	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, TEXT("Prototype_GameObject_Camera_Free"),
		LEVEL_GAMEPLAY, pLayerTag, &Desc)))
		return E_FAIL;

	return S_OK;
}

HRESULT Level_GamePlay::Ready_Lights()
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

Level_GamePlay* Level_GamePlay::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Level_GamePlay* pInstance = new Level_GamePlay(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Level_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Level_GamePlay::Free()
{
	__super::Free();
}
