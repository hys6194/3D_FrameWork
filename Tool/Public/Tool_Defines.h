#pragma once

#include <Windows.h>
#include <process.h>

#include <Vfw.h>
#pragma comment(lib, "vfw32.lib")

#include <io.h>
#include <shlobj.h> 

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/ImGuizmo.h"

#include "Tool_ConstValue.h"

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



//string WstrToStr(const std::wstring& wide_str)
//{
//	string narrow_str;
//
//	int size_needed = (CP_ACP, 0, wide_str.c_str(), -1, NULL, 0, NULL, NULL);
//	if (size_needed > 0)
//	{
//		narrow_str.resize(size_needed - 1);
//		(CP_ACP, 0, wide_str.c_str(), -1, &narrow_str[0], size_needed, NULL, NULL);
//	}
//
//	return narrow_str;
//}
//
//wstring StrToWstr(const string& narrow_str)
//{
//	wstring wide_str(narrow_str.length() + 1, L'\0');
//	size_t converted_chars = 0;
//
//	mbstowcs_s(&converted_chars, &wide_str[0], wide_str.size(), narrow_str.c_str(), narrow_str.length());
//	return wide_str;
//}
//
//wstring Get_FileName(const wstring& path)
//{
//	size_t slashPos = path.find_last_of(L'\\');
//
//	if (slashPos == std::wstring::npos)
//		slashPos = path.find_last_of(L'/');
//
//	wstring filename = (slashPos == wstring::npos) ? path : path.substr(slashPos + 1);
//
//	size_t dotPos = filename.find_last_of(L'.');
//
//	if (dotPos != std::wstring::npos)
//		filename = filename.substr(0, dotPos);
//
//	return filename;
//}