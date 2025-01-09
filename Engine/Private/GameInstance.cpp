#include "GameInstance.h"

#include "Graphic_Device.h"
#include "Timer_Manager.h"
#include "AbstractFactory.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{
	//m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, ppDevice, ppContext);
	//if (nullptr == m_pGraphic_Device)
	//	return E_FAIL;
	//
	//if(FAILED(AbstractFactory::Create<CTimer_Manager>(m_pTimer_Manager))) return E_FAIL;
	// 
	//m_pTimer_Manager = CTimer_Manager::Create();
	//if (nullptr == m_pTimer_Manager)
	//	return E_FAIL;

	FAILED_CHECK_RETURN(m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, ppDevice, ppContext), E_FAIL);
	FAILED_CHECK_RETURN((AbstractFactory::Create<CTimer_Manager>(m_pTimer_Manager)), E_FAIL);
	//FAILED_CHECK_RETURN(m_pTimer_Manager = CTimer_Manager::Create(), E_FAIL);

	return S_OK;
}	
#pragma region GRAPHIC_DEVICE
HRESULT CGameInstance::Clear_BackBuffer_View(_float4 vClearColor)
{
	return m_pGraphic_Device->Clear_BackBuffer_View(vClearColor);
}

HRESULT CGameInstance::Clear_DepthStencil_View()
{
	return m_pGraphic_Device->Clear_DepthStencil_View();
}

HRESULT CGameInstance::Present()
{
	return m_pGraphic_Device->Present();
}
#pragma endregion
#pragma region TIMER_MANAGER
_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);	
}

void CGameInstance::Set_TimeDelta(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Set_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Ready_Timer(strTimerTag);
}
#pragma endregion

void CGameInstance::Free()
{
	__super::Free();

	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pTimer_Manager);
}
