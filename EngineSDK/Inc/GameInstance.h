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

class ENGINE_DLL GameInstance final : public Base
{
	DECLARE_SINGLETON(GameInstance)

private:
	GameInstance();
	virtual ~GameInstance() = default;

public:
	HRESULT				Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void				Update_Engine(_float fTimeDelta);
	void				Draw_Engine();
	void				Release_Engine();
	void				Clear(_uint iLevelIndex);

	_float				Random_Normalize();
	_float				Random(_float fMin, _float fMax);


#pragma region GRAPHIC_DEVICE
	HRESULT				Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT				Clear_DepthStencil_View();
	HRESULT				Present();
#pragma endregion

#pragma region INPUT_DEVICE
	_byte				Get_DIKeyState(_ubyte byKeyID);
	_byte				Get_DIMouseState(MOUSEKEYSTATE eMouse);
	_long				Get_DIMouseMove(MOUSEMOVESTATE eMouseState);

#pragma endregion

#pragma region TIMER_MANAGER
public:
	_float				Get_TimeDelta(const _wstring& strTimerTag);
	void				Set_TimeDelta(const _wstring& strTimerTag);
	HRESULT				Add_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
	HRESULT				Open_Level(_uint iLevelIndex, class Level* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT				Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, Base* pPrototype);

	// 왜 pArg = nullptr로 디폴트 인자값으로? -> pArg가 필요할 수도 없을수도 있기 때문에
	Base*				Clone_Prototype(PROTOTYPE ePrototypeType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region Object_MANAGER
	HRESULT				Add_GameObject(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	class Layer*		Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag);
#pragma endregion

#pragma region RENDERER
	HRESULT				Add_RenderObject(Renderer::RENDERERGROUP eRenderGroupID, class GameObject* pRenderObject);
#pragma endregion

#pragma region PIPELINE
	const _float4x4*	Get_Transform_Float4x4(PipeLine::TRANSFORMSTATE eState);
	_matrix				Get_Transform_Matrix(PipeLine::TRANSFORMSTATE eState);
	const _float4x4*	Get_Transform_Inverse_Float4x4(PipeLine::TRANSFORMSTATE eState) const;
	_matrix				Get_Transform_Inverse_Matrix(PipeLine::TRANSFORMSTATE eState) const;
	const _float4*		Get_CamPosition() const;
	void				Set_Transform(PipeLine::TRANSFORMSTATE eState, _fmatrix Matrix);
	void				Set_Transform(PipeLine::TRANSFORMSTATE eState, const _float4x4* pMatrix);
	HRESULT				Bind_VP_Transform_SR(const _char* pConstantName, Shader* pShader, PipeLine::TRANSFORMSTATE eState);
#pragma endregion


#pragma region Light_Manager
	HRESULT				Add_Light(const LIGHT_DESC& pDesc);
	const LIGHT_DESC*	Get_LightDesc(_uint iLightIndex)const;
#pragma endregion



private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CInput_Device*			m_pInput_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class Level_Manager*			m_pLevel_Manager = { nullptr };
	class Prototype_Manager*		m_pPrototype_Manager = { nullptr };
	class Object_Manager*			m_pObject_Manager = { nullptr };
	class Renderer*					m_pRenderer = { nullptr };	
	class PipeLine*					m_pPipeLine = { nullptr };
	class Light_Manager*			m_pLight_Manager = { nullptr };

public:	
	virtual void Free() override;
};

END