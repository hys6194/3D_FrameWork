#include "GameInstance.h"
#include "AbstractFactory.h"

#include "Graphic_Device.h"
#include "Object_Manager.h"	
#include "Layer.h"
#include "GameObject.h"
#include "Timer_Manager.h"
#include "Level_Manager.h"
#include "Input_Device.h"
#include "PipeLine.h"
#include "Light_Manager.h"


IMPLEMENT_SINGLETON(GameInstance)

GameInstance::GameInstance()
{

}

HRESULT GameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{

	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, ppDevice, ppContext);
	NULL_CHECK_RETURN(m_pGraphic_Device, E_FAIL);

	m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd);
	NULL_CHECK_RETURN(m_pInput_Device, E_FAIL);

	m_pTimer_Manager = CTimer_Manager::Create();
	NULL_CHECK_RETURN(m_pTimer_Manager, E_FAIL);

	m_pPrototype_Manager = Prototype_Manager::Create(*ppDevice, *ppContext, EngineDesc.iNumLevels);
	NULL_CHECK_RETURN(m_pPrototype_Manager, E_FAIL);

	m_pLevel_Manager = Level_Manager::Create();
	NULL_CHECK_RETURN(m_pLevel_Manager, E_FAIL);

	m_pObject_Manager = Object_Manager::Create(EngineDesc.iNumLevels);
	NULL_CHECK_RETURN(m_pObject_Manager, E_FAIL);

	m_pRenderer = Renderer::Create(*ppDevice, *ppContext);
	NULL_CHECK_RETURN(m_pRenderer, E_FAIL);

	m_pPipeLine = PipeLine::Create();
	NULL_CHECK_RETURN(m_pPipeLine, E_FAIL);

	m_pLight_Manager = Light_Manager::Create(*ppDevice, *ppContext);
	NULL_CHECK_RETURN(m_pLight_Manager, E_FAIL);
	

	//FAILED_CHECK_RETURN 사용 못함 : 주소가 짤리는 듯함

	//FAILED_CHECK_RETURN(m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, ppDevice, ppContext), E_FAIL);
	//FAILED_CHECK_RETURN(m_pLevel_Manager = Level_Manager::Create(), E_FAIL);
	//FAILED_CHECK_RETURN(m_pObject_Manager = Object_Manager::Create(EngineDesc.iNumLevels), E_FAIL);

	return S_OK;
}

void GameInstance::Update_Engine(_float fTimeDelta)
{
	m_pInput_Device->Update();

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pPipeLine->Update();
	m_pObject_Manager->Update(fTimeDelta);
	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);
}

void GameInstance::Draw_Engine()
{
	if (nullptr == m_pRenderer)
		return;

	m_pRenderer->Draw();
}

void GameInstance::Clear(_uint iLevelIndex)
{
	// 추후 오브젝트, 컴포넌트들의 Clear함수들을 여기서 호출하여 정리할 것

 	m_pRenderer->Clear();

	m_pObject_Manager->Clear(iLevelIndex);

	m_pPrototype_Manager->Clear(iLevelIndex);

}

_float GameInstance::Random_Normalize()
{
	// 0과 1사이의 소수를 뽑아내기 위한 함수
	return static_cast<_float>(rand()) / RAND_MAX;
}

_float GameInstance::Random(_float fMin, _float fMax)
{
	return fMin + Random_Normalize() * (fMax - fMin);
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

#pragma region INPUT_DEVICE

_byte GameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}
_byte GameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}
_long GameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);

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

#pragma endregion

#pragma region OBJECT_MANAGER

HRESULT GameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

Layer* GameInstance::Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);
}

GameObject* GameInstance::Find_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, const _tchar* strObjectTag)
{
	return m_pObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, strObjectTag);
}

#pragma endregion

#pragma region RENDERER

HRESULT GameInstance::Add_RenderObject(Renderer::RENDERERGROUP eRenderGroupID, GameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderObject(eRenderGroupID, pRenderObject);
}

#pragma endregion

#pragma region PIPELINE

const _float4x4* GameInstance::Get_Transform_Float4x4(PipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}
_matrix GameInstance::Get_Transform_Matrix(PipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

const _float4x4* GameInstance::Get_Transform_Inverse_Float4x4(PipeLine::TRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Float4x4(eState);
}

_matrix GameInstance::Get_Transform_Inverse_Matrix(PipeLine::TRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Matrix(eState);
}

const _float4* GameInstance::Get_CamPosition() const
{
	return m_pPipeLine->Get_CamPosition();
}

void GameInstance::Set_Transform(PipeLine::TRANSFORMSTATE eState, _fmatrix Matrix)
{
	return m_pPipeLine->Set_Transform(eState, Matrix);
}
void GameInstance::Set_Transform(PipeLine::TRANSFORMSTATE eState, const _float4x4* pMatrix)
{
	return m_pPipeLine->Set_Transform(eState, pMatrix);
}
HRESULT GameInstance::Bind_VP_Transform_SR(const _char* pConstantName, Shader* pShader, PipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Bind_SR(pShader, pConstantName, eState);
}

#pragma endregion

#pragma region Light_Manager

HRESULT GameInstance::Add_Light(const LIGHT_DESC& pDesc)
{
	return m_pLight_Manager->Add_Light(pDesc);
}
const LIGHT_DESC* GameInstance::Get_LightDesc(_uint iLightIndex) const
{
	return m_pLight_Manager->Get_LightDesc(iLightIndex);
}
#pragma endregion

void GameInstance::Release_Engine()
{
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pInput_Device);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pPrototype_Manager);		
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pPipeLine);
	Safe_Release(m_pLight_Manager);

	GameInstance::DestroyInstance();
}


void GameInstance::Free()
{
	__super::Free();
}
