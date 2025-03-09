#ifndef InputDev_h__
#define InputDev_h__

#include "Base.h"

BEGIN(Engine)

class  CInput_Device : public Base
{
private:
	CInput_Device(void);
	virtual ~CInput_Device(void) = default;

public:
	_byte	Get_DIKeyState(_ubyte byKeyID)
	{
		return m_byCurKeyState[byKeyID];
	}

	_byte	Get_DIMouseState(MOUSEKEYSTATE eMouse)
	{
		return m_tCurMouseState.rgbButtons[static_cast<_uint>(eMouse)];
	}

	// 현재 마우스의 특정 축 좌표를 반환
	_long	Get_DIMouseMove(MOUSEMOVESTATE eMouseState)
	{
		return *((reinterpret_cast<_long*>(&m_tCurMouseState)) + static_cast<_uint>(eMouseState));
	}

	_long	Get_DIMouseWheel() { return m_tCurMouseState.lZ; }

public:
	_bool Key_Pressing(_uint iKeyID);
	_bool Key_Down(_uint iKeyID);
	_bool Key_Up(_uint iKeyID);

public:
	_bool	Mouse_Down(MOUSEKEYSTATE eMouse);
	_bool	Mouse_Drag(MOUSEKEYSTATE eMouse);
	_bool	Mouse_Up(MOUSEKEYSTATE eMouse);


public:
	HRESULT Initialize(HINSTANCE hInst, HWND hWnd);
	void	Update(void);

private:
	LPDIRECTINPUT8			m_pInputSDK = { nullptr };

private:
	LPDIRECTINPUTDEVICE8	m_pKeyBoard = { nullptr };
	LPDIRECTINPUTDEVICE8	m_pMouse = { nullptr };

private:
	_byte					m_byCurKeyState[256];		
	_byte					m_byPrevKeyState[256];				// 키보드에 있는 모든 키값을 저장하기 위한 변수

	DIMOUSESTATE2			m_tCurMouseState	= { 0 };
	DIMOUSESTATE2			m_tPrevMouseState	= { 0 };

public:
	static CInput_Device* Create(HINSTANCE hInstance, HWND hWnd);
	virtual void Free(void);

};
END
#endif // InputDev_h__


