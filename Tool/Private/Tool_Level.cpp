#include "Tool_Level.h"
#include "GameInstance.h"

#include "Tool_FreeCam.h"
#include "Tool_Manager.h"

Tool_Level::Tool_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CLevel{ pDevice , pContext }
{
}

HRESULT Tool_Level::Initialize()
{
	if (FAILED(Ready_Lights()))
		return E_FAIL;

	if (FAILED(Ready_Layer_Player(TEXT("Layer_Player"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Camera(TEXT("Layer_Camera"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_BackGround(TEXT("Layer_Terrain"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Navi(TEXT("Layer_Monster"))))
		return E_FAIL;

	if (FAILED(Ready_Layer_Object(TEXT("Layer_Objcet"))))
		return E_FAIL;

	m_pTool = Tool_Manager::Create(m_pDevice, m_pContext, m_pGameInstance);

    return S_OK;
}

void Tool_Level::Update(_float fTimeDelta)
{
	//SetWindowText(g_hWnd, TEXT("현재 레벨 : 툴 레벨"));

	m_pTool->Update(fTimeDelta);

	m_pTool->Late_Update(fTimeDelta);
}

HRESULT Tool_Level::Render()
{
	

    return S_OK;
}

HRESULT Tool_Level::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TOOL, PRO_OBJ_TERRAIN,
		LEVEL_TOOL, pLayerTag)))
		return E_FAIL;
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TOOL, PRO_OBJ_SKY,
	//	LEVEL_TOOL, pLayerTag)))
	//	return E_FAIL;

    return S_OK;
}

HRESULT Tool_Level::Ready_Layer_Camera(const _tchar* pLayerTag)
{
	CTool_FreeCam::CAMERA_FREE_DESC   FreeCam_Desc{};
	
	FreeCam_Desc.vEye = _float3(0.f, 20.f, -20.f);
	FreeCam_Desc.vAt = _float3(0.f, 0.f, 0.f);
	FreeCam_Desc.fFov = XMConvertToRadians(60.f);
	FreeCam_Desc.fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	FreeCam_Desc.fNear = 0.1f;
	FreeCam_Desc.fFar = 300.f;
	FreeCam_Desc.fMouseSensor = 0.05f;
	lstrcpy(FreeCam_Desc.szGameObjectTag, TEXT("GameObject_Camera_Free"));
	FreeCam_Desc.fSpeedPerSec = 10.f;
	FreeCam_Desc.fRotationPerSec = XMConvertToRadians(90.f);
	
	if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TOOL, PRO_OBJ_CAM_FREE,
		LEVEL_TOOL, pLayerTag, &FreeCam_Desc)))
		return E_FAIL;

    return S_OK;
}

HRESULT Tool_Level::Ready_Layer_Navi(const _tchar* pLayerTag)
{
    return S_OK;
}

HRESULT Tool_Level::Ready_Layer_Player(const _tchar* pLayerTag)
{
    return S_OK;
}

HRESULT Tool_Level::Ready_Layer_Object(const _tchar* pLayerTag)
{
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TOOL, PRO_OBJ_ROCK1,
	//	LEVEL_TOOL, pLayerTag)))
	//	return E_FAIL;
	
	//if (FAILED(m_pGameInstance->Add_GameObject(LEVEL_TOOL, PRO_OBJ_ROCK2,
	//	LEVEL_TOOL, pLayerTag)))
	//	return E_FAIL;

	return S_OK;
}

HRESULT Tool_Level::Ready_Tool_Manager()
{
	

	return S_OK;
}

HRESULT Tool_Level::Ready_Lights()
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

Tool_Level* Tool_Level::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Tool_Level* pInstance = new Tool_Level(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Tool_Level");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Tool_Level::Free()
{
	__super::Free();

	Safe_Release(m_pTool);
}
