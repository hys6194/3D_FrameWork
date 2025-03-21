#include "ImGui_Map.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/ImGuizmo.h"

#include "GameInstance.h"
#include "Map_Object.h"


CImGui_Map::CImGui_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImGui_Base { pDevice , pContext }
{
}

HRESULT CImGui_Map::Initialize()
{
	ImGui::SetNextWindowSize(ImVec2(500, 400));
	ImGui::SetNextWindowPos(ImVec2(1280, 0));
	ImGui::SetNextWindowCollapsed(false);

	return S_OK;
}

void CImGui_Map::Update(_float fTimeDelta)
{
	ImGui::Begin("Map_Windows");

	Default_SetButtons(fTimeDelta);

	Get_PrototypeList(TEXT("Model"), TEXT("GameObject"));

	ImGui::End();
}

HRESULT CImGui_Map::Late_Update(_float fTimeDelta)
{
	


	return S_OK;
}

void CImGui_Map::Default_SetButtons(_float fTimeDelta)
{
	if (ImGui::Button("Save"))
	{
		int a = 10;
	}

	ImGui::SameLine();

	if (ImGui::Button("Load"))
	{

	}

	ImGui::SameLine();

	if (ImGui::Button("+"))
	{
		CMap_Object::MAPOBJ_DESC Desc = {};

		Desc.strModelTag = m_strModelName;

		// 오브젝트의 이름은 진짜 어떻게 해야함? 나도 모름
		lstrcpy(Desc.szGameObjectTag, TEXT("Layer_Desert_Rock"));

		m_pGameInstance->Add_GameObject(LEVEL_TOOL, m_strObjectName, LEVEL_TOOL, TEXT("Layer_Objcet"), &Desc);
	}

	ImGui::SameLine();

	if (ImGui::Button("-"))
	{
		//m_pGameInstance->Find_Layer
	}

}

CImGui_Map* CImGui_Map::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CImGui_Map* pInstance = new CImGui_Map(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed_Create_Map_Windows");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Map::Free()
{
	__super::Free();

}
