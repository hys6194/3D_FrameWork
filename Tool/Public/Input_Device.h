#ifndef InputDev_h__
#define InputDev_h__

#include "pch.h"
#include "Base.h"

BEGIN(Engine)

class  CInput_Device : public Base
{
private:
										CInput_Device(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual								~CInput_Device(void) = default;

public:
	_byte								Get_DIKeyState(_ubyte byKeyID)
	{
		return m_byCurKeyState[byKeyID];
	}

	_byte								Get_DIMouseState(MOUSEKEYSTATE eMouse)
	{
		return m_tMouseState.rgbButtons[static_cast<_uint>(eMouse)];
	}
	
	// 현재 마우스의 특정 축 좌표를 반환
	_long								Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *(((_long*)&m_tMouseState) + static_cast<_uint>(eMouseState));
	}
	_long								Get_DIMouseWheel() { return m_tMouseState.lZ; }

public:
	_bool								Key_Pressing(_uint iKeyID);
	_bool								Key_Down(_uint iKeyID);
	_bool								Key_Up(_uint iKeyID);

public:
	_bool								Mouse_Down(MOUSEKEYSTATE eMouse);
	_bool								Mouse_Drag(MOUSEKEYSTATE eMouse);
	_bool								Mouse_Up(MOUSEKEYSTATE eMouse);


public:
	HRESULT								Initialize(HINSTANCE g_hInstance, HWND hWnd);
	void								Update(void);

private:
	LPDIRECTINPUT8						m_pInputSDK			= { nullptr };
	LPDIRECTINPUTDEVICE8				m_pKeyBoard			= { nullptr };
	LPDIRECTINPUTDEVICE8				m_pMouse			= { nullptr };
	ID3D11Device*						m_pDevice			= { nullptr };
	ID3D11DeviceContext*				m_pContext			= { nullptr };

private:
	_byte								m_byCurKeyState		[256];		
	_byte								m_byPrevKeyState	[256];				// 키보드에 있는 모든 키값을 저장하기 위한 변수

private:
	DIMOUSESTATE						m_tMouseState		= {};
	DIMOUSESTATE2						m_tCurMouseState	= { 0 };
	DIMOUSESTATE2						m_tPrevMouseState	= { 0 };

private:
	class CGameInstance*				m_pGameInstance		= {nullptr};

private:
	HWND								m_phWnd				= { nullptr };
	HINSTANCE							m_hInstance			= { nullptr };

public:
	static CInput_Device*				Create(HINSTANCE hInstance, HWND hWnd, _bool isWindowed, _uint iWinSizeX, _uint iWinSizeY, ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual void						Free(void);

};
END
#endif // InputDev_h__


