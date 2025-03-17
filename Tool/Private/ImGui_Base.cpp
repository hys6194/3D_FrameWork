#include "ImGui_Base.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/ImGuizmo.h"

#include "GameInstance.h"

CImGui_Base::CImGui_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext{ pContext }
	, m_pGameInstance { CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT CImGui_Base::Read_Objects(const _tchar* pFileName, MODELTYPE eType)
{


	return S_OK;
}

HRESULT CImGui_Base::Save_Objects()
{
	return S_OK;
}

HRESULT CImGui_Base::Load_Objects()
{
	

	return S_OK;
}

HRESULT CImGui_Base::Mouse_Picking()
{
	return S_OK;
}

void CImGui_Base::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
