#pragma once

#include <Windows.h>

/* 현재 프로젝트에서 사용하기위한 공통적인 정의를 모아둔다. */
namespace Client
{
	static const unsigned int	g_iWinSizeX = 1280;
	static const unsigned int	g_iWinSizeY = 720;
}

extern HINSTANCE		g_hInstance;
extern HWND				g_hWnd;

using namespace Client;
