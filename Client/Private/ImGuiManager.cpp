#include "pch.h"

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx9.h"

#include "ImGuiManager.h"

IMPLEMENT_SINGLETON(CImGuiManager)

CImGuiManager::CImGuiManager() {}

CImGuiManager::~CImGuiManager() { Free(); }

 void CImGuiManager::SetUp_ImguiManager(LPDIRECT3DDEVICE9 _Device)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(g_hWnd);
	ImGui_ImplDX9_Init(_Device);
}

void CImGuiManager::CleanUp_ImguiManager()
{
	ImGui_ImplDX9_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

void CImGuiManager::Render_Begin()
{
	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
}

void CImGuiManager::Render_DemoWindow()
{
	bool bDemo = true;
	ImGui::ShowDemoWindow(&bDemo);
	ImGui::Render();
	ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

void CImGuiManager::Render_CameraEditor() {}

void CImGuiManager::Free() {}