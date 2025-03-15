#pragma once

#include <Windows.h>
#include <process.h>

namespace Tool
{
	static const unsigned int	g_iWinSizeX = 1280;
	static const unsigned int	g_iWinSizeY = 720;

enum LEVEL { LEVEL_STATIC, LEVEL_MENU, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_TOOL, LEVEL_END };	

}

extern HINSTANCE		g_hInstance;
extern HWND				g_hWnd;


using namespace Tool;
using namespace std;
