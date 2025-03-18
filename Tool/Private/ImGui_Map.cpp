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

	// 여기서 모델을 담아라? 차라리 모델을 가져오는게 훨 나아보이는데

	// 흐음 필요는 없어보이긴한데 차라리 파일에 있는 fbx 파일을
	// 읽어 들이는게 훨 나을 수도?
	// 근데 어차피 원형객체를 만들어 놓잖아
	// 그러면 원형 객체 목록을 옆에 띄워 놓고
	// 클릭해서 맵에 둘 때, 사본으로 Clone하는것으로 하자고
	// 그냥 GameInstance를 통해서 가져오는 것이 좋아보임
	// 차라리 여기서 vector를 채우고 꺼내서 쓰는게 좋아보임




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
		CMap_Object::MAPOBJ_DESC Desc = {};

		Desc.strModelTag = m_strModelName;

		// 오브젝트의 이름은 진짜 어떻게 해야함? 나도 모름
		lstrcpy(Desc.szGameObjectTag, TEXT("Layer_Desert_Rock"));

		m_pGameInstance->Add_GameObject(LEVEL_TOOL, m_strObjectName, LEVEL_TOOL, TEXT("Layer_Objcet"), &Desc);

	}

	SameLine();

	if (Button("+"))
	{
		CMap_Object::MAPOBJ_DESC Desc = {};
	
		Desc.strModelTag = PRO_MODEL_ROCK2;
	
		lstrcpy(Desc.szGameObjectTag, TEXT("Layer_Desert_Rock2"));
	
		m_pGameInstance->Add_GameObject(LEVEL_TOOL, PRO_OBJ_ROCK2, LEVEL_TOOL, TEXT("Layer_Objcet"), &Desc);
	}

	if (Button("-"))
	{
		//m_pGameInstance->Add_GameObject
	}

}

CImGui_Map* CImGui_Map::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CImGui_Map* pInstance = new CImGui_Map(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed_Create_UI_Windows");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CImGui_Map::Free()
{
	__super::Free();

}
