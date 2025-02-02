#include "GameInstance.h"

#include "Graphic_Device.h"
#include "Timer_Manager.h"
#include "AbstractFactory.h"
#include "Level_Manager.h"

IMPLEMENT_SINGLETON(GameInstance)

GameInstance::GameInstance()
{

}

HRESULT GameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{

	FAILED_CHECK_RETURN(m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, ppDevice, ppContext), E_FAIL);
	FAILED_CHECK_RETURN(m_pTimer_Manager = CTimer_Manager::Create(), E_FAIL);
	FAILED_CHECK_RETURN(m_pLevel_Manager = Level_Manager::Create(), E_FAIL);

	//m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, ppDevice, ppContext);
	//if (nullptr == m_pGraphic_Device)
	//	return E_FAIL;
	//
	//m_pTimer_Manager = CTimer_Manager::Create();
	//if (nullptr == m_pTimer_Manager)
	//	return E_FAIL;
	//
	//m_pLevel_Manager = Level_Manager::Create();
	//if (nullptr == m_pLevel_Manager)
	//	return E_FAIL;

	return S_OK;
}

void GameInstance::Update_Engine(_float fTimeDelta)
{
	m_pLevel_Manager->Update(fTimeDelta);
}

void GameInstance::Clear(_uint iLevelIndex)
{
	// 추후 오브젝트, 컴포넌트들의 Clear함수들을 여기서 호출하여 정리할 것
}

#pragma region GRAPHIC_DEVICE
HRESULT GameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT GameInstance::Clear_DepthStencil_View()
{
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT GameInstance::Present()
{
	return m_pGraphic_Device->Present();
}
#pragma endregion

#pragma region TIMER_MANAGER
_float GameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);	
}

void GameInstance::Set_TimeDelta(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Set_TimeDelta(strTimerTag);
}

HRESULT GameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Ready_Timer(strTimerTag);
}
#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT GameInstance::Open_Level(_uint iLevelIndex, Level* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNewLevel);
}

#pragma endregion

void GameInstance::Release_Engine()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);

	GameInstance::DestroyInstance();
}


void GameInstance::Free()
{
	__super::Free();
}
