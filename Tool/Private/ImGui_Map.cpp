#include "ImGui_Map.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/ImGuizmo.h"

#include "GameInstance.h"
#include "Map_Object.h"

using namespace ImGui;

CImGui_Map::CImGui_Map(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CImGui_Base { pDevice , pContext }
{
}

HRESULT CImGui_Map::Initialize()
{
	SetNextWindowSize(ImVec2(500, 400));
	SetNextWindowPos(ImVec2(1280, 0));
	SetNextWindowCollapsed(false);

	return S_OK;
}

void CImGui_Map::Update(_float fTimeDelta)
{
	Begin("Map_Windows");

	Default_SetButtons(fTimeDelta);

	Get_PrototypeList(TEXT("Model"), TEXT("GameObject"));

	End();
}

HRESULT CImGui_Map::Late_Update(_float fTimeDelta)
{
	


	return S_OK;
}

void CImGui_Map::Default_SetButtons(_float fTimeDelta)
{
	if (Button("Save"))
	{
		int a = 10;
	}

	SameLine();

	if (Button("Load"))
	{

	}

	SameLine();

	if (Button("+"))
	{
		CMap_Object::MAPOBJ_DESC Desc = {};

		Desc.strModelTag = m_strModelName;

		// 오브젝트의 이름은 진짜 어떻게 해야함? 나도 모름
		lstrcpy(Desc.szGameObjectTag, TEXT("Layer_Desert_Rock"));

		m_pGameInstance->Add_GameObject(LEVEL_TOOL, m_strObjectName, LEVEL_TOOL, TEXT("Layer_Objcet"), &Desc);
	}

	SameLine();

	if (Button("-"))
	{
		//m_pGameInstance->Find_Layer
	}

	SameLine();

	if (Button("TTTT"))
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
