#pragma once

//#define NOMINMAX

#include <Windows.h>
#include <process.h>

#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include <io.h>

#include "../Bin/fmod/fmod.h"
#include "../Bin/fmod/fmod.hpp"
#pragma comment(lib, "../Bin/fmod/fmodex_vc.lib")

/* 현재 프로젝트에서 사용하기위한 공통적인 정의를 모아둔다. */
namespace Client
{
	static const unsigned int	g_iWinSizeX = 1280;
	static const unsigned int	g_iWinSizeY = 720;

	enum LEVEL { LEVEL_STATIC, LEVEL_MENU, LEVEL_LOGO, LEVEL_GAMEPLAY, LEVEL_LOADING, LEVEL_END };

}

#define TIME60			TEXT("Timer_60")
#define AXIS_X			XMVectorSet(1.f, 0.f, 0.f, 0.f)
#define AXIS_Y			XMVectorSet(0.f, 1.f, 0.f, 0.f)
#define AXIS_Z			XMVectorSet(0.f, 0.f, 1.f, 0.f)
#define AXIS_W			XMVectorSet(0.f, 0.f, 0.f, 1.f)
 

extern HINSTANCE		g_hInstance;
extern HWND				g_hWnd;

using namespace Client;
using namespace std;
