#pragma once

#include "Renderer.h"
#include "PipeLine.h"
#include "Prototype.h"


/* GameInstance */
/* 엔진이 제공해주는 대부분의 기능을 모아둔다. */
/* 한번의 함수 호출로 대부분의 초기화 기능을 한꺼번에 수행한다. */
/* 한번의 함수 호출로 대부분의 기능을 정리한다. */
// 따라서 GameInstance는 싱글톤으로 생성

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT							Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void							Update_Engine(_float fTimeDelta);
	void							Draw_Engine();
	void							Release_Engine();
	void							Clear(_uint iLevelIndex);

	_float							Random_Normalize();
	_float							Random(_float fMin, _float fMax);


#pragma region GRAPHIC_DEVICE
	HRESULT							Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT							Clear_DepthStencil_View();
	HRESULT							Present();
	void							Set_RenderTergets(_int iNumTarget);
#pragma endregion

#pragma region INPUT_DEVICE
	_byte							Get_DIKeyState(_ubyte byKeyID);
	_byte							Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long							Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

	_bool							Key_Pressing(_uint iKeyID);
	_bool							Key_Down(_uint iKeyID);
	_bool							Key_Up(_uint iKeyID);

	_bool							Mouse_Down(MOUSEKEYSTATE eMouse);
	_bool							Mouse_Drag(MOUSEKEYSTATE eMouse);
	_bool							Mouse_Up(MOUSEKEYSTATE eMouse);

#pragma endregion

#pragma region IMGUI_MANAGER
	void							SetUp_Render_ImGui();
	void							EndRender_ImGui();
	void							SetUp_ImGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd);
	//vector<float*>				Draw_Gizmo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _matrix& matWorld);
	void							Set_EndMsg();
	void							Render_ImGui();

#pragma endregion


#pragma region TIMER_MANAGER
public:
	_float							Get_TimeDelta(const _wstring& strTimerTag);
	void							Set_TimeDelta(const _wstring& strTimerTag);
	HRESULT							Add_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
	HRESULT							Open_Level(_uint iLevelIndex, class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT							Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CBase* pPrototype);
	HRESULT							Collect_ProtoTag(_uint iLevelIndex);
	vector<wstring>*				Get_PrototypeTag(_uint iLevelIndex);
	// 왜 pArg = nullptr로 디폴트 인자값으로? -> pArg가 필요할 수도 없을수도 있기 때문에
	CBase*							Clone_Prototype(PROTOTYPE ePrototypeType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region Object_MANAGER
	HRESULT							Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	class CLayer*					Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
	CGameObject*					Find_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, const _tchar* strObjectTag);
#pragma endregion

#pragma region RENDERER
	HRESULT							Add_RenderObject(CRenderer::RENDERERGROUP eRenderGroupID, class CGameObject* pRenderObject);
#pragma endregion

#pragma region PIPELINE
	const _float4x4*				Get_Transform_Float4x4(CPipeLine::TRANSFORMSTATE eState);
	_matrix							Get_Transform_Matrix(CPipeLine::TRANSFORMSTATE eState);
	const _float4x4*				Get_Transform_Inverse_Float4x4(CPipeLine::TRANSFORMSTATE eState) const;
	_matrix							Get_Transform_Inverse_Matrix(CPipeLine::TRANSFORMSTATE eState) const;
	const _float4*					Get_CamPosition() const;
	const _float4*					Get_MouseWindowPosition() const;
	_vector*						Get_MouseWorldPosition(const _float4x4* TargetmatWorld);
	void							Set_Transform(CPipeLine::TRANSFORMSTATE eState, _fmatrix Matrix);
	void							Set_Transform(CPipeLine::TRANSFORMSTATE eState, const _float4x4* pMatrix);
	HRESULT							Bind_VP_Transform_SR(const _char* pConstantName, CShader* pShader, CPipeLine::TRANSFORMSTATE eState);
#pragma endregion

#pragma region Light_Manager
	HRESULT							Add_Light(const LIGHT_DESC& pDesc);
	const LIGHT_DESC*				Get_LightDesc(_uint iLightIndex)const;
#pragma endregion

#pragma region Font_Manager
	HRESULT							Add_Font(const _wstring& strFontTag, const _tchar* pFontFilePath);
	HRESULT							Draw_Text(const _wstring& strFontTag, const _wstring& strText, const _float2& vPosition = _float2(0.f, 0.f), _fvector vColor = XMVectorSet(1.f, 1.f, 1.f, 1.f), _float fRadian = 0.f, const _float2& vOrigin = _float2(0.f, 0.f), _float fScale = 1.f);
#pragma endregion

private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*		m_pPrototype_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };	
	class CPipeLine*				m_pPipeLine = { nullptr };
	class CLight_Manager*			m_pLight_Manager = { nullptr };
	class CFont_Manager*			m_pFont_Manager = { nullptr };
	class CImGui_Manager*			m_pImGui_Manager = { nullptr };

public:	
	virtual void					Free() override;
};

END