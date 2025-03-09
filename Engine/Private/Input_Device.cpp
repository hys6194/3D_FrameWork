
#include "Input_Device.h"

Engine::CInput_Device::CInput_Device(void)
{
	ZeroMemory(m_byCurKeyState, sizeof(m_byCurKeyState));
	ZeroMemory(m_byPrevKeyState, sizeof(m_byPrevKeyState));
}


_bool CInput_Device::Key_Pressing(_uint iKeyID)
{
	return (m_byCurKeyState[iKeyID] & 0x80) && (m_byPrevKeyState[iKeyID] & 0x80);
}

_bool CInput_Device::Key_Down(_uint iKeyID)
{
	return (m_byCurKeyState[iKeyID] & 0x80) && !(m_byPrevKeyState[iKeyID] & 0x80);
}

_bool CInput_Device::Key_Up(_uint iKeyID)
{
	return !(m_byCurKeyState[iKeyID] & 0x80) && (m_byPrevKeyState[iKeyID] & 0x80);
}

_bool CInput_Device::Mouse_Down(MOUSEKEYSTATE eMouse)
{
	return (m_tCurMouseState.rgbButtons[eMouse] & 0x80) && !(m_tPrevMouseState.rgbButtons[eMouse] & 0x80);
}

_bool CInput_Device::Mouse_Drag(MOUSEKEYSTATE eMouse)
{
	return (m_tCurMouseState.rgbButtons[eMouse] & 0x80) && (m_tPrevMouseState.rgbButtons[eMouse] & 0x80);
}

_bool CInput_Device::Mouse_Up(MOUSEKEYSTATE eMouse)
{
	return !(m_tCurMouseState.rgbButtons[eMouse] & 0x80) && (m_tPrevMouseState.rgbButtons[eMouse] & 0x80);
}

HRESULT Engine::CInput_Device::Initialize(HINSTANCE hInst, HWND hWnd)
{

	// DInput 컴객체를 생성하는 함수
	FAILED_CHECK_RETURN(DirectInput8Create(hInst,
											DIRECTINPUT_VERSION,
											IID_IDirectInput8,
											(void**)&m_pInputSDK,
											NULL), E_FAIL);

	// 키보드 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysKeyboard, &m_pKeyBoard, nullptr), E_FAIL);

	// 생성된 키보드 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pKeyBoard->SetDataFormat(&c_dfDIKeyboard);

	// 장치에 대한 독점권을 설정해주는 함수, (클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수)
	m_pKeyBoard->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pKeyBoard->Acquire();


	// 마우스 객체 생성
	FAILED_CHECK_RETURN(m_pInputSDK->CreateDevice(GUID_SysMouse, &m_pMouse, nullptr), E_FAIL);

	// 생성된 마우스 객체의 대한 정보를 컴 객체에게 전달하는 함수
	m_pMouse->SetDataFormat(&c_dfDIMouse);

	// 장치에 대한 독점권을 설정해주는 함수, 클라이언트가 떠있는 상태에서 키 입력을 받을지 말지를 결정하는 함수
	m_pMouse->SetCooperativeLevel(hWnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);

	// 장치에 대한 access 버전을 받아오는 함수
	m_pMouse->Acquire();


	return S_OK;
}

void Engine::CInput_Device::Update(void)
{
	//m_pKeyBoard->GetDeviceState(256, m_byKeyState);
	//m_pMouse->GetDeviceState(sizeof(m_tMouseState), &m_tMouseState);

	memcpy(m_byPrevKeyState, m_byCurKeyState, sizeof(m_byCurKeyState));
	m_pKeyBoard->GetDeviceState(0xff + 1, m_byCurKeyState);

	m_tPrevMouseState = m_tCurMouseState;
	m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE2), &m_tCurMouseState);
}

CInput_Device* CInput_Device::Create(HINSTANCE hInstance, HWND hWnd)
{
	CInput_Device* pInstance = new CInput_Device();

	if (FAILED(pInstance->Initialize(hInstance, hWnd)))
	{
		MSG_BOX("Failed to Created : CInput_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Engine::CInput_Device::Free(void)
{
	Safe_Release(m_pKeyBoard);
	Safe_Release(m_pMouse);
	Safe_Release(m_pInputSDK);
}

