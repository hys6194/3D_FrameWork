#pragma once

#include "Base.h"
#include "AbstractFactory.h"

/* CGameInstance */
/* 엔진이 제공해주는 대부분의 기능을 모아둔다. */
/* 한번의 함수 호출로 대부분의 초기화 기능을 한꺼번에 수행한다. */
/* 한번의 함수 호출로 대부분의 기능을 정리한다. */

BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance)

private:
	CGameInstance();
	virtual ~CGameInstance() = default;


public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, ID3D11Device** ppDevice, ID3D11DeviceContext** ppContext);


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


private:
	class CGraphic_Device* m_pGraphic_Device = { nullptr };
	class CTimer_Manager* m_pTimer_Manager = { nullptr };

public:	
	virtual void Free() override;
};

END