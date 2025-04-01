#include "ImGui_Manager.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/ImGuizmo.h"


CImGui_Manager::CImGui_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pDevice);
}

HRESULT CImGui_Manager::Initialize(HWND hWnd)
{
	SetUp_ImGui(m_pDevice, m_pContext, hWnd);

	return S_OK;
}

void CImGui_Manager::SetUp_ImGui(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
#ifdef USING_IMPLOT
	ImPlot::CreateContext();
#endif

	ImGui::StyleColorsDark();

	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows
	// 얘는 왠지는 모르겠지만 DX11상에서는 안되는듯함

	ImGuiStyle& style = ImGui::GetStyle();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(pDevice, pContext);

}

void CImGui_Manager::Update_ImGui_Windows(_float fTimeDelta)
{

}

void CImGui_Manager::Late_Update_ImGui_Windows(_float fTimeDelta)
{

}

void CImGui_Manager::Render()
{
	

}

void CImGui_Manager::SetUp_Render_ImGui()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	ImGuizmo::BeginFrame();
	
	// Update를 돌게 하는 함수를 넣어야 함

	//_uint i = 1;
	//m_pContext->RSGetViewports(&i, &m_pViewPort);
	//
	//
	//ImGuiIO& io = ImGui::GetIO();
	//
	//ImGuizmo::SetDrawlist();
	//ImGuizmo::SetRect(0, 0, io.DisplaySize.x, io.DisplaySize.x);
	//ImGuizmo::SetOrthographic(false);
	//ImGuizmo::SetDrawlist();
	//ImGuizmo::SetRect(0, 0, m_pViewPort.Width, m_pViewPort.Height);


	//ImGuizmo::Manipulate(
	//	(float*)&m_matView,
	//	(float*)&m_matProj,
	//	eGizmoType,
	//	ImGuizmo::WORLD,
	//	(float*)&m_matWorld);

	Create_DockingSpace();
}

//void CImGui_Manager::Render()
//{
//	// 추후 Update에서 이벤트가 갱신되면 추가로 갱신할 수 있게 해야하는 
//  //Late_Update 자리로 한다
//
//}

void CImGui_Manager::EndRender_ImGui()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable
		&& m_bEndMsg == false)
	{
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

}

vector<float*> CImGui_Manager::Draw_Gizmo(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd, _matrix& matWorld)
{

	return vector<float*>();
}

void CImGui_Manager::Create_DockingSpace()
{
	const ImGuiViewport* viewport = ImGui::GetMainViewport();

	ImGui::SetNextWindowPos(viewport->WorkPos);
	ImGui::SetNextWindowSize(viewport->WorkSize);
	ImGui::SetNextWindowViewport(viewport->ID);

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_NoBackground;
	static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

	dockspace_flags |= ImGuiDockNodeFlags_PassthruCentralNode;
	ImGui::Begin("DockSpace Demo", 0, window_flags);

	ImGui::PopStyleVar();
	ImGui::PopStyleVar(2);

	ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);

	ImGui::End();
}

CImGui_Manager* CImGui_Manager::Create(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CImGui_Manager* pInstance = new CImGui_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize(hWnd)))
	{
		MSG_BOX("Failed to Created : CImGui_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Manager::Release_ImGui()
{
	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
#ifdef USING_IMPLOT
	ImPlot::DestroyContext();
#endif
}

void CImGui_Manager::Free()
{
	__super::Free();

	Release_ImGui();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
