#include "ImGui_UI.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/ImGuizmo.h"

using namespace ImGui;

CImGui_UI::CImGui_UI(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImGui_Base { pDevice , pContext }
{
}

HRESULT CImGui_UI::Initialize()
{
	SetNextWindowSize(ImVec2(500, 400));
	SetNextWindowPos(ImVec2(100, 100));
	SetNextWindowCollapsed(false);

	return S_OK;
}

void CImGui_UI::Update(_float fTimeDelta)
{
	Begin("UI_Windows");



	End();
}

HRESULT CImGui_UI::Late_Update(_float fTimeDelta)
{
	


	return S_OK;
}

CImGui_UI* CImGui_UI::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CImGui_UI* pInstance = new CImGui_UI(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed_Create_UI_Windows");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_UI::Free()
{
	__super::Free();
}
