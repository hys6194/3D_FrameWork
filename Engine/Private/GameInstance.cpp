#include "GameInstance.h"
#include "AbstractFactory.h"

#include "Graphic_Device.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Object_Manager.h"	


IMPLEMENT_SINGLETON(GameInstance)

GameInstance::GameInstance()
{

}

HRESULT GameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{

	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, ppDevice, ppContext);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;

	m_pPrototype_Manager = Prototype_Manager::Create(*ppDevice, *ppContext, EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;	

	m_pLevel_Manager = Level_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;

	m_pObject_Manager = Object_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pObject_Manager)
		return E_FAIL;

	m_pRenderer = Renderer::Create(*ppDevice, *ppContext);
	if (nullptr == m_pRenderer)
		return E_FAIL;
	

	//FAILED_CHECK_RETURN 사용 못함 : 주소가 짤리는 듯함

	//FAILED_CHECK_RETURN(m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, ppDevice, ppContext), E_FAIL);
	//FAILED_CHECK_RETURN(m_pLevel_Manager = Level_Manager::Create(), E_FAIL);
	//FAILED_CHECK_RETURN(m_pObject_Manager = Object_Manager::Create(EngineDesc.iNumLevels), E_FAIL);

	return S_OK;
}

void GameInstance::Update_Engine(_float fTimeDelta)
{
	m_pLevel_Manager->Update(fTimeDelta);
}

void GameInstance::Draw_Engine()
{
	m_pRenderer->Draw();
}

void GameInstance::Clear(_uint iLevelIndex)
{
	// 추후 오브젝트, 컴포넌트들의 Clear함수들을 여기서 호출하여 정리할 것

 	m_pRenderer->Clear();

	m_pObject_Manager->Clear(iLevelIndex);

	m_pPrototype_Manager->Clear(iLevelIndex);

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

#pragma region PROTOTYPE_MANAGER

HRESULT GameInstance::Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, Base* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

Base* GameInstance::Clone_Prototype(PROTOTYPE ePrototypeType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototypeType, iLevelIndex, strPrototypeTag, pArg);
}
HRESULT GameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}
HRESULT GameInstance::Add_RenderObject(Renderer::RENDERERGROUP eRenderGroupID, GameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderObject(eRenderGroupID, pRenderObject);
}
#pragma endregion

#pragma region OBJECT_MANAGER

#pragma endregion

void GameInstance::Release_Engine()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pRenderer);

	GameInstance::DestroyInstance();
}


void GameInstance::Free()
{
	__super::Free();
}
