#pragma once

#include "Base.h"

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
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);
	void Update_Engine(_float fTimeDelta);
	void Release_Engine();
	void Clear(_uint iLevelIndex);


#pragma region GRAPHIC_DEVICE
	HRESULT Clear_BackBuffer_View(_float4 vClearColor);	
	HRESULT Clear_DepthStencil_View();
	HRESULT Present();
#pragma endregion

#pragma region TIMER_MANAGER
public:
	_float			Get_TimeDelta(const _wstring& strTimerTag);
	void			Set_TimeDelta(const _wstring& strTimerTag);
	HRESULT			Add_Timer(const _wstring& strTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
	HRESULT Open_Level(_uint iLevelIndex, class Level* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, Base* pPrototype);

	// 왜 pArg = nullptr로 디폴트 인자값으로? -> pArg가 필요할 수도 없을수도 있기 때문에
	Base* Clone_Prototype(PROTOTYPE ePrototypeType, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region Object_MANAGER

#pragma endregion



private:
	class CGraphic_Device* m_pGraphic_Device = { nullptr };
	class CTimer_Manager* m_pTimer_Manager = { nullptr };
	class Level_Manager* m_pLevel_Manager = { nullptr };
	class Prototype_Manager* m_pPrototype_Manager = { nullptr };
	class Object_Manager* m_pObject_Manager = { nullptr };

public:	
	virtual void Free() override;
};

END