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
#include "Font_Manager.h"
#include "ImGui_Manager.h"
#include "Target_Manager.h"
#include "CollisionManager.h"
#include "Picking.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{

}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext)
{

	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, ppDevice, ppContext);
	NULL_CHECK_RETURN(m_pGraphic_Device, E_FAIL);

	m_pInput_Device = CInput_Device::Create(EngineDesc.hInstance, EngineDesc.hWnd, EngineDesc.isWindowed, EngineDesc.iWidth_VP, EngineDesc.iHeight_VP, *ppDevice, *ppContext);
	NULL_CHECK_RETURN(m_pInput_Device, E_FAIL);

	m_pTimer_Manager = CTimer_Manager::Create();
	NULL_CHECK_RETURN(m_pTimer_Manager, E_FAIL);

	m_pPrototype_Manager = CPrototype_Manager::Create(*ppDevice, *ppContext, EngineDesc.iNumLevels);
	NULL_CHECK_RETURN(m_pPrototype_Manager, E_FAIL);

	m_pLevel_Manager = CLevel_Manager::Create();
	NULL_CHECK_RETURN(m_pLevel_Manager, E_FAIL);

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels);
	NULL_CHECK_RETURN(m_pObject_Manager, E_FAIL);

	m_pTarget_Manager = CTarget_Manager::Create(*ppDevice, *ppContext);
	NULL_CHECK_RETURN(m_pTarget_Manager, E_FAIL);

	m_pRenderer = CRenderer::Create(*ppDevice, *ppContext);
	NULL_CHECK_RETURN(m_pRenderer, E_FAIL);

	m_pPipeLine = CPipeLine::Create(*ppDevice, *ppContext,EngineDesc.hWnd);
	NULL_CHECK_RETURN(m_pPipeLine, E_FAIL);

	m_pLight_Manager = CLight_Manager::Create(*ppDevice, *ppContext);
	NULL_CHECK_RETURN(m_pLight_Manager, E_FAIL);

	m_pFont_Manager = CFont_Manager::Create(*ppDevice, *ppContext);
	NULL_CHECK_RETURN(m_pFont_Manager, E_FAIL);

	m_pImGui_Manager = CImGui_Manager::Create(EngineDesc.hWnd, *ppDevice, *ppContext);
	NULL_CHECK_RETURN(m_pImGui_Manager, E_FAIL);

	m_pCollision_Manager = CCollision_Manager::Create();
	NULL_CHECK_RETURN(m_pCollision_Manager, E_FAIL);

	m_pPicking = CPicking::Create(*ppDevice, *ppContext, EngineDesc.hWnd);
	NULL_CHECK_RETURN(m_pPicking, E_FAIL);
	
	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pImGui_Manager->SetUp_Render_ImGui();


	m_pInput_Device->Update();

	m_pPicking->Copy(TARGET_PICK);
	// 여기에서 콜리젼 매니져를 통해 삭제처리가 되어야 하는 애들을 삭제 처리
	// 혹은 렌더링 기능을 끄게 설정

	m_pObject_Manager->Priority_Update(fTimeDelta);
	m_pPipeLine->Update();
	m_pObject_Manager->Update(fTimeDelta);

	m_pImGui_Manager->Update_ImGui_Windows(fTimeDelta);

	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pCollision_Manager->Update_Collisions(fTimeDelta);

	m_pLevel_Manager->Update(fTimeDelta);


}

void CGameInstance::Draw_Engine()
{
	if (nullptr == m_pRenderer)
		return;
		
	m_pRenderer->Draw();

	m_pImGui_Manager->EndRender_ImGui();
	m_pGraphic_Device->Set_RenderTargets(1);

}

void CGameInstance::Clear(_uint iLevelIndex)
{
	// 추후 오브젝트, 컴포넌트들의 Clear함수들을 여기서 호출하여 정리할 것

 	m_pRenderer->Clear();

	m_pObject_Manager->Clear(iLevelIndex);

	m_pPrototype_Manager->Clear(iLevelIndex);

}

_float CGameInstance::Random_Normalize()
{
	// 0과 1사이의 소수를 뽑아내기 위한 함수
	return static_cast<_float>(rand()) / RAND_MAX;
}

_float CGameInstance::Random(_float fMin, _float fMax)
{
	return fMin + Random_Normalize() * (fMax - fMin);
}

_bool CGameInstance::Random_Persent(_uint iSuccessProb)
{
	if (0 > iSuccessProb)
		iSuccessProb = 0;

	random_device rand;
	mt19937 gen1(rand());

	_uint iLow, iHigh;
	iLow = 1;
	iHigh = 100;

	uniform_real_distribution<> Probability(iLow, iHigh);

	_uint iPersent = (_uint)Probability(gen1);

	if (iPersent <= iSuccessProb)
		return true;
	else
		return false;
}

_uint CGameInstance::Draw_RandomNum(_uint iNumber)
{
	if (0 > iNumber)
		iNumber = 0;

	random_device rand;
	mt19937 gen1(rand());

	_uint iLow, iHigh;
	iLow = 1;
	iHigh = iNumber;

	uniform_int_distribution<>Index(iLow, iHigh);

	_uint iIndex = (_uint)Index(gen1);

	return iIndex;
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

void CGameInstance::Set_RenderTergets(_int iNumTarget)
{
	return m_pGraphic_Device->Set_RenderTargets(iNumTarget);
}

#pragma endregion

#pragma region INPUT_DEVICE

_byte CGameInstance::Get_DIKeyState(_ubyte byKeyID)
{
	return m_pInput_Device->Get_DIKeyState(byKeyID);
}

_byte CGameInstance::Get_DIMouseState(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Get_DIMouseState(eMouse);
}

_long CGameInstance::Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
{
	return m_pInput_Device->Get_DIMouseMove(eMouseState);

}

_bool CGameInstance::Key_Pressing(_uint iKeyID)
{
	return m_pInput_Device->Key_Pressing(iKeyID);
}

_bool CGameInstance::Key_Down(_uint iKeyID)
{
	return m_pInput_Device->Key_Down(iKeyID);
}

_bool CGameInstance::Key_Up(_uint iKeyID)
{
	return m_pInput_Device->Key_Up(iKeyID);
}

_bool CGameInstance::Mouse_Down(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Mouse_Down(eMouse);
}

_bool CGameInstance::Mouse_Drag(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Mouse_Drag(eMouse);
}

_bool CGameInstance::Mouse_Up(MOUSEKEYSTATE eMouse)
{
	return m_pInput_Device->Mouse_Up(eMouse);
}
#pragma endregion

#pragma region IMGUI_MANAGER

void CGameInstance::SetUp_Render_ImGui()
{
	m_pImGui_Manager->SetUp_Render_ImGui();
}
void CGameInstance::EndRender_ImGui()
{
	m_pImGui_Manager->EndRender_ImGui();
}
void CGameInstance::SetUp_ImGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	m_pImGui_Manager->SetUp_ImGui(pDevice, pContext, hWnd);
}
vector<float*> CGameInstance::Draw_Gizmo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _matrix& matWorld)
{
	return m_pImGui_Manager->Draw_Gizmo(pDevice, pContext, hWnd, matWorld);
}
void CGameInstance::Set_EndMsg()
{
	m_pImGui_Manager->Set_EndMsg();
}

void CGameInstance::Render_ImGui()
{
	m_pImGui_Manager->Render();
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

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Open_Level(_uint iLevelIndex, CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(iLevelIndex, pNewLevel);
}

#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

HRESULT CGameInstance::Collect_ProtoTag(_uint iLevelIndex)
{
	return m_pPrototype_Manager->Collect_PrototypeTag(iLevelIndex);
}

vector<wstring>* CGameInstance::Get_PrototypeTag(_uint iLevelIndex)
{
	return m_pPrototype_Manager->Get_PrototypeTag(iLevelIndex);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototypeType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototypeType, iLevelIndex, strPrototypeTag, pArg);
}

#pragma endregion

#pragma region OBJECT_MANAGER

HRESULT CGameInstance::Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject(iPrototypeLevelIndex, strPrototypeTag, iLevelIndex, strLayerTag, pArg);
}

HRESULT CGameInstance::Delete_LastObject(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Delete_LastLayer(iLevelIndex, strLayerTag);
}

CLayer* CGameInstance::Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Find_Layer(iLevelIndex, strLayerTag);
}

CGameObject* CGameInstance::Find_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, const _tchar* strObjectTag)
{
	return m_pObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, strObjectTag);
}

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);
}

list<CGameObject*>* CGameInstance::Get_GameObjectList(_uint iLevelIndex, const _wstring& strLayerTag)
{
	return m_pObject_Manager->Get_GameObjectList(iLevelIndex, strLayerTag);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderObject(CRenderer::RENDERERGROUP eRenderGroupID, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderObject(eRenderGroupID, pRenderObject);
}

#ifdef _DEBUG
void CGameInstance::Add_Renderer_DebugComponent(CComponent* pDebugComponent)
{
	return m_pRenderer->Add_DebugComponent(pDebugComponent);
}

#endif

#pragma endregion

#pragma region PIPELINE

const _float4x4* CGameInstance::Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Float4x4(eState);
}
_matrix CGameInstance::Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Get_Transform_Matrix(eState);
}

const _float4x4* CGameInstance::Get_Transform_Inverse_Float4x4(CPipeLine::TRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Float4x4(eState);
}

_matrix CGameInstance::Get_Transform_Inverse_Matrix(CPipeLine::TRANSFORMSTATE eState) const
{
	return m_pPipeLine->Get_Transform_Inverse_Matrix(eState);
}

const _float4* CGameInstance::Get_CamPosition() const
{
	return m_pPipeLine->Get_CamPosition();
}

_vector* CGameInstance::Get_MouseWindowPosition()
{
	return m_pPipeLine->Get_MouseWindowPosition();
}

_vector* CGameInstance::Get_PlayerViewPortPos()
{
	return m_pPipeLine->Get_PlayerViewPortPos();
}

_float4* CGameInstance::Get_RayDirCoords()
{
	return m_pPipeLine->Get_RayDirCoords();
}

vector<_float4>* CGameInstance::Get_RayCoords()
{
	return m_pPipeLine->Get_RayCoords();
}

void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix Matrix)
{
	return m_pPipeLine->Set_Transform(eState, Matrix);
}
void CGameInstance::Set_Transform(CPipeLine::TRANSFORMSTATE eState, const _float4x4* pMatrix)
{
	return m_pPipeLine->Set_Transform(eState, pMatrix);
}
HRESULT CGameInstance::Bind_VP_Transform_SR(const _char* pConstantName, CShader* pShader, CPipeLine::TRANSFORMSTATE eState)
{
	return m_pPipeLine->Bind_SR(pShader, pConstantName, eState);
}

#pragma endregion

#pragma region Light_Manager

HRESULT CGameInstance::Add_Light(const LIGHT_DESC& pDesc)
{
	return m_pLight_Manager->Add_Light(pDesc);
}
HRESULT CGameInstance::Render_Light(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pLight_Manager->Render(pShader, pVIBuffer);
}
const LIGHT_DESC* CGameInstance::Get_LightDesc(_uint iLightIndex) const
{
	return m_pLight_Manager->Get_LightDesc(iLightIndex);
}
#pragma endregion


#pragma region Font_Manager

HRESULT CGameInstance::Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath)
{
	return m_pFont_Manager->Add_Font(strFontTag, pFontFilePath);
}
HRESULT CGameInstance::Draw_Text(const _wstring& strFontTag, const _wstring& strText, const _float2& vPosition, _fvector vColor, _float fRadian, const _float2& vOrigin, _float fScale)
{
	return m_pFont_Manager->Render(strFontTag, strText, vPosition, vColor, fRadian, vOrigin, fScale);
}

#pragma endregion

#pragma region Collision_Manager

HRESULT CGameInstance::Add_Collistionlist(const _uint iCollOption, const wstring& strColliderTag, CBounding* pInstance)
{
	return m_pCollision_Manager->Add_Collistionlist(iCollOption, strColliderTag, pInstance);
}

HRESULT CGameInstance::Regist_Update(class CBounding* pCollCom1, class CBounding* pCollCom2)
{
	return m_pCollision_Manager->Regist_Update(pCollCom1, pCollCom2);
}

HRESULT CGameInstance::Secede_Update(class CBounding* pCollCom1, class CBounding* pCollCom2)
{
	return m_pCollision_Manager->Secede_Update(pCollCom1, pCollCom2);
}

#pragma endregion

#pragma region TARGET_MANAGER

HRESULT CGameInstance::Add_RenderTarget(const _wstring& strTargetTag, _uint iSizeX, _uint iSizeY, DXGI_FORMAT ePixelFormat, const _float4& vClearColor)
{
	return m_pTarget_Manager->Add_RenderTarget(strTargetTag, iSizeX, iSizeY, ePixelFormat, vClearColor);
}

HRESULT CGameInstance::Add_MRT(const _wstring& strMRTTag, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Add_MRT(strMRTTag, strTargetTag);
}

HRESULT CGameInstance::Bind_RT_ToShader(CShader* pShader, const _char* pConstantName, const _wstring& strTargetTag)
{
	return m_pTarget_Manager->Bind_SR(pShader, pConstantName, strTargetTag);
}

HRESULT CGameInstance::Begin_MRT(const _wstring& strMRTTag)
{
	return m_pTarget_Manager->Begin_MRT(strMRTTag);
}

void CGameInstance::Copy_RenderTarget(const _wstring& strTargetTag, ID3D11Texture2D* pTexture2D)
{
	return m_pTarget_Manager->Copy_RenderTarget(strTargetTag, pTexture2D);
}

HRESULT CGameInstance::End_MRT()
{
	return m_pTarget_Manager->End_MRT();
}

#ifdef _DEBUG
HRESULT CGameInstance::Ready_RT_Debug(const _wstring& strTargetTag, _float fX, _float fY, _float fSizeX, _float fSizeY)
{
	return m_pTarget_Manager->Ready_Debug(strTargetTag, fX, fY, fSizeX, fSizeY);
}
HRESULT CGameInstance::Render_RT_Debug(const _wstring& strMRTTag, CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
	return m_pTarget_Manager->Render(strMRTTag, pShader, pVIBuffer);
}
_bool CGameInstance::Picking(_float3* pOut)
{
	return m_pPicking->Picking(pOut);
}
#endif

#pragma endregion

void CGameInstance::Release_Engine()
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
	Safe_Release(m_pFont_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pTarget_Manager);
	Safe_Release(m_pPicking);
	Safe_Release(m_pImGui_Manager);


	CGameInstance::DestroyInstance();
}


void CGameInstance::Free()
{
	__super::Free();


}
