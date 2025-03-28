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

void CImGui_Map::Save_MapObjects()
{
}

void CImGui_Map::Set_TransformInfo(CMap_Object* pObject)
{
	m_pObject = pObject;
	m_pTransform = m_pObject->Get_Transform();
}

void CImGui_Map::Render_TransformInfo()
{
	// 여기에서 Scale, Rotation, Pos에 관한 정보들을 담아볼 것
	//_vector vLook = m_pTransform->Get_State(CTransform::STATE_LOOK);
	//_vector vRight = m_pTransform->Get_State(CTransform::STATE_RIGHT);
	//_vector vUp = m_pTransform->Get_State(CTransform::STATE_UP);
	//_vector vPos = m_pTransform->Get_State(CTransform::STATE_POS);
	//
	//// 스케일 설정 방법
	//m_pObject->Get_Transform()->SetUp_Scaled(1.f, 1.f, 1.f);
	//
	//// 행렬 정보 세팅 방법
	//_vector vTest{1.f,1.f,1.f,1.f};
	//m_pTransform->Set_State(CTransform::STATE_RIGHT, vTest);
	//m_pTransform->Set_State(CTransform::STATE_UP, vTest);
	//m_pTransform->Set_State(CTransform::STATE_POS, vTest);
	//m_pTransform->Set_State(CTransform::STATE_LOOK, vTest);

	Button_TransformScale();


}

void CImGui_Map::Default_SetButtons(_float fTimeDelta)
{
	if (Button("Save"))
	{
		Save_MapObjects();
		m_bSave = true;
	}

	SameLine();

	if (Button("Load"))
	{
		m_bLoad = true;
	}

	Button_AddObjects();

	Button_DeleleObjects();

	Button_NaviCreate();
		
	if (m_bSelect)
		Change_ObjectInfo();
}

void CImGui_Map::Button_AddObjects()
{
	SameLine();

	if (Button("+") && m_strModelName != L"")
	{
		CMap_Object::MAPOBJ_DESC Desc = {};

		Desc.strModelTag = m_strModelName;
		Desc.strObjectTag = m_strObjectName;

		HRESULT hr = m_pGameInstance->Add_GameObject(LEVEL_TOOL, m_strObjectName, LEVEL_TOOL, TEXT("Layer_Objcet"), &Desc);

		// 생성 실패해도 증가되는거라 문제되긴한데 흐음
		if(hr != E_FAIL)
		{
			m_iObjCnt++;
			m_listObject = m_pGameInstance->Get_GameObjectList(LEVEL_TOOL, TEXT("Layer_Objcet"));
		}

		// 여기서 생성할 때마다 list를 업데이트하자 
		// 어떻게 가져올 것이냐
	}
}

void CImGui_Map::Button_DeleleObjects()
{
	SameLine();

	if (Button("-") && m_iObjCnt > 0)
	{
		HRESULT hr = m_pGameInstance->Delete_LastObject(LEVEL_TOOL, TEXT("Layer_Objcet"));

		if(S_OK == hr)
			m_iObjCnt--;
	}
}

void CImGui_Map::Button_NaviCreate()
{
	SameLine(255.f, 0.f);

	if (Checkbox("Navi", &m_bNavi))
	{
	}
}

void CImGui_Map::Button_TransformScale()
{
	//const _tchar szScale;
		
	_float4 fScale =
	{
		m_pTransform->Get_WorldMatrix_Ptr()->m[0][0],
		m_pTransform->Get_WorldMatrix_Ptr()->m[1][1],
		m_pTransform->Get_WorldMatrix_Ptr()->m[2][2],
		m_pTransform->Get_WorldMatrix_Ptr()->m[3][3],
	};

	Button_Info(fScale);

}

void CImGui_Map::Button_TransformRotation()
{

}

void CImGui_Map::Button_TransformPosition()
{

}

void CImGui_Map::Change_ObjectInfo()
{
	Begin("Object_Transforms");

	BulletText("Scale");
	Render_TransformInfo();



	int a = 10;

	End();
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
