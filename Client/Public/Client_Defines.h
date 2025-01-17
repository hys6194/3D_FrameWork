#pragma once

#include <Windows.h>
#include <process.h>

/* 현재 프로젝트에서 사용하기위한 공통적인 정의를 모아둔다. */
namespace Client
{
	static const unsigned int	g_iWinSizeX = 1280;
	static const unsigned int	g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END };

}

extern HINSTANCE		g_hInstance;
extern HWND				g_hWnd;

using namespace Client;
using namespace std;
