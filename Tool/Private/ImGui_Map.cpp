#include "ImGui_Map.h"

#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_win32.h"
#include "ImGui/imgui_impl_dx11.h"
#include "ImGui/ImGuizmo.h"


#include "GameInstance.h"
#include "Map_Object.h"
#include "Tool_FreeCam.h"

 
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


	// 키 누르면 마지막 오브젝트로의 카메라 이동
	if (m_pGameInstance->Key_Pressing(DIK_GRAVE))
	{
		_vector vPos = m_pObject->Get_Transform()->Get_State(CTransform::STATE_POS);

		_float4 fPos{};

		XMStoreFloat4(&fPos, vPos);

		fPos.x = fPos.x * m_pObject->Get_Transform()->Update_Scale().x;
		fPos.y = (fPos.y + 30) * m_pObject->Get_Transform()->Update_Scale().x;
		fPos.z = (fPos.z - 30) * m_pObject->Get_Transform()->Update_Scale().x;

		m_pGameInstance->Find_GameObject(
			LEVEL_TOOL,
			TEXT("Layer_Camera"),
			TEXT("GameObject_Camera_Free"))->
			Get_Transform()->Set_State(CTransform::STATE_POS, XMLoadFloat4(&fPos));

	}

	End();
}

HRESULT CImGui_Map::Late_Update(_float fTimeDelta)
{
	return S_OK;
}

void CImGui_Map::Save_MapObjects()
{
	//list<CMap_Object*> plist = dynamic_cast<CMap_Object*>(m_listObject);
	list<CMap_Object*>* listObjects = reinterpret_cast<list<CMap_Object*>*>(m_listObject);
	CMap_Object::MAPOBJ_DESC Desc{};

	_ulong			dwByte = {};
	HANDLE			hFile = CreateFile(TEXT("../../Client/Bin/DataFiles/MapObjectData.dat"), GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return;

	_uint iMapObjCount = listObjects->size();

	WriteFile(hFile, &iMapObjCount, sizeof(_uint), &dwByte, nullptr);

	for (auto iter = listObjects->begin();
		iter != listObjects->end();
		iter++)
	{
		Desc = (*iter)->Get_MapObjDesc();

		MODELTYPE eModelType = Desc.eType;
		WriteFile(hFile, &eModelType, sizeof(eModelType), &dwByte, nullptr);

		_wstring strModelTag = Desc.strModelTag;
		size_t  iModelTagLen = strModelTag.length();
		WriteFile(hFile, &iModelTagLen, sizeof(iModelTagLen), &dwByte, nullptr);
		WriteFile(hFile, strModelTag.data(), iModelTagLen * sizeof(wchar_t), &dwByte, nullptr);

		_wstring strObjectTag = Desc.strObjectTag;
		size_t  iObjectTagLen = strObjectTag.length();
		WriteFile(hFile, &iObjectTagLen, sizeof(iObjectTagLen), &dwByte, nullptr);
		WriteFile(hFile, strObjectTag.data(), iObjectTagLen * sizeof(wchar_t), &dwByte, nullptr);

		_float4x4 matWorld = *(*iter)->Get_Transform()->Get_WorldMatrix_Ptr();
		WriteFile(hFile, &matWorld, sizeof(matWorld), &dwByte, nullptr);
		
	}

	CloseHandle(hFile);
}

void CImGui_Map::Load_MapObjects()
{
	_ulong          dwByte = {};
	CMap_Object::MAPOBJ_DESC Desc{};
	HANDLE          hFile = CreateFile(TEXT("../../Client/Bin/DataFiles/MapObjectData.dat"), GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (0 == hFile)
		return;

	_uint iMapObjCount;
	ReadFile(hFile, &iMapObjCount, sizeof(_uint), &dwByte, nullptr);

	for (size_t i = 0; i < iMapObjCount; i++)
	{
		// 모델 타입
		MODELTYPE eModelType;
		ReadFile(hFile, &eModelType, sizeof(eModelType), &dwByte, nullptr);

		// 모델 이름
		size_t  iModelTagLen;
		ReadFile(hFile, &iModelTagLen, sizeof(iModelTagLen), &dwByte, nullptr);

		wstring strModelTag(iModelTagLen, L'\0');
		ReadFile(hFile, &strModelTag[0], iModelTagLen * sizeof(wchar_t), &dwByte, nullptr);

		// 오브젝트 이름
		size_t  iObjectTagLen;
		ReadFile(hFile, &iObjectTagLen, sizeof(iObjectTagLen), &dwByte, nullptr);

		wstring strObjectTag(iObjectTagLen, L'\0');
		ReadFile(hFile, &strObjectTag[0], iObjectTagLen * sizeof(wchar_t), &dwByte, nullptr);

		// 월드 행렬
		_float4x4 matWorld;
		ReadFile(hFile, &matWorld, sizeof(matWorld), &dwByte, nullptr);

		int a = 10;

		Desc.eType = eModelType;
		Desc.strModelTag = strModelTag;
		Desc.strObjectTag = strObjectTag;
		Desc.matWorld = matWorld;
		Desc.m_bIsLoad = true;

		m_pGameInstance->Add_GameObject(LEVEL_TOOL, Desc.strObjectTag, LEVEL_TOOL, TEXT("Layer_Objcet"), &Desc);

		
	}

	CloseHandle(hFile);

	m_listObject = m_pGameInstance->Get_GameObjectList(LEVEL_TOOL, TEXT("Layer_Objcet"));
}

void CImGui_Map::Set_TransformInfo(CMap_Object* pObject)
{
	m_pObject = pObject;
	m_pTransform = m_pObject->Get_Transform();
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
		Load_MapObjects();
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

		Desc.eType				= MODELTYPE::TYPE_NONANIM;
		Desc.strModelTag		= m_strModelName;
		Desc.strObjectTag		= m_strObjectName;
		Desc.fRotValue			= { 0.f,0.f,0.f };
		Desc.iObjectIndex		= m_iObjCnt;

		HRESULT hr = m_pGameInstance->Add_GameObject(LEVEL_TOOL, m_strObjectName, LEVEL_TOOL, TEXT("Layer_Objcet"), &Desc);

		// 생성 실패해도 증가되는거라 문제되긴한데 흐음
		if(hr != E_FAIL)
		{
			m_iObjCnt++;
			m_listObject = m_pGameInstance->Get_GameObjectList(LEVEL_TOOL, TEXT("Layer_Objcet"));
		}
		// 생성할 때 마다 해당 오브젝트의 transform을 가져와서 편하게 하자

		auto iter = m_listObject->back();
		m_pObject = dynamic_cast<CMap_Object*>(iter);
		m_pTransform = m_pObject->Get_Transform();

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
		int a = 1;
	}

	if(m_pGameInstance->Key_Down(DIK_R))
	{
		m_bReverse = !m_bReverse;
	}

	char cInfoX[MAX_PATH]{};
	sprintf_s(cInfoX, sizeof(cInfoX), "x : %s", m_bReverse ? "true" : "false");
	Text(cInfoX);

}

void CImGui_Map::Button_TransformPosition()
{
	_float4 fPosition;
	XMStoreFloat4(&fPosition, m_pTransform->Get_State(CTransform::STATE_POS));

	Button_Info(fPosition);

}

void CImGui_Map::Change_ObjectInfo()
{
	Begin("Object_Transforms");

	_float4x4 pMatrix = *m_pTransform->Get_WorldMatrix_Ptr();
	XMMatrixDecompose(&m_vScale, &m_vRotation, &m_vTranslation, XMLoadFloat4x4(&pMatrix));


	// 스케일
	BulletText("Scale");
	SameLine();
	char cInfoX[MAX_PATH]{};
	sprintf_s(cInfoX, sizeof(cInfoX), "x : %s", m_bAll ? "true" : "false");
	Text(cInfoX);
	Render_TransformScale();

	// 회전
	BulletText("Rotation");
	Render_TransformRotation();

	// 포지션
	BulletText("Position");
	Render_TransformPosition();


	End();

}

void CImGui_Map::Render_TransformScale()
{
	_float4 fScale{};
	XMStoreFloat4(&fScale, m_vScale);

	Button_Info(fScale);

	SameLine();
	if (Button("All"))
	{
		m_bAll = !m_bAll;
		m_fScale = fScale.x;
	}

	else if(m_bEvent1)
	{
		m_fScale = fScale.x;
	}
		
	else if (m_bEvent2)
	{
		m_fScale = fScale.y;
	}
	else if (m_bEvent3)
	{
		m_fScale = fScale.z;
	}


	SameLine(270, 0);
	BulletText("Set_Scale");


	SameLine(360, 0);
	PushItemWidth(50.0f);

	// X, Y, Z Scale 조정
	if (InputFloat("##Set_Scale", &m_fScale) &&
		m_pGameInstance->Key_Down(DIK_RETURN))
	{
		// 0 예외처리
		if (m_fScale == 0)
			return;

		if(m_bAll)
			m_pTransform->SetUp_Scaled(m_fScale, m_fScale, m_fScale);

		else if (m_bEvent1)
			m_pTransform->SetUp_Scaled(m_fScale, fScale.y, fScale.z);
		else if (m_bEvent2)
			m_pTransform->SetUp_Scaled(fScale.x, m_fScale, fScale.z);
		else if (m_bEvent3)
			m_pTransform->SetUp_Scaled(fScale.x, fScale.y, m_fScale);


	}
	ImGui::PopItemWidth();

	
	if (ImGui::SliderFloat("Scale", &m_fScale, m_fScale - 0.1f, m_fScale + 0.1f))
	{
		if (m_fScale <= 0)
			m_fScale = 0.1f;

		if (m_bAll)
			m_pTransform->SetUp_Scaled(m_fScale, m_fScale, m_fScale);

		else if (m_bEvent1)
			m_pTransform->SetUp_Scaled(m_fScale, fScale.y, fScale.z);
		else if (m_bEvent2)
			m_pTransform->SetUp_Scaled(fScale.x, m_fScale, fScale.z);
		else if (m_bEvent3)
			m_pTransform->SetUp_Scaled(fScale.x, fScale.y, m_fScale);
	}

}

void CImGui_Map::Render_TransformRotation()
{

	_float4 fRotation = {
	m_pObject->Get_MapObjDesc().fRotValue.x,
	m_pObject->Get_MapObjDesc().fRotValue.y,
	m_pObject->Get_MapObjDesc().fRotValue.z,
	1.f
	};
	//_float4 fRotation{};
	//
	//XMStoreFloat4(&fRotation, m_vRotation);


	Button_Info(fRotation);
	
	if (m_bAll)
	{
		int a = 10;
	}

	else if (m_bEvent1)
	{	
		m_fValue = m_pObject->Get_MapObjDesc().fRotValue.x;
	}
	else if (m_bEvent2)
	{
		m_fValue = m_pObject->Get_MapObjDesc().fRotValue.y;
	}
	else if (m_bEvent3)
	{
		m_fValue = m_pObject->Get_MapObjDesc().fRotValue.z;
	}


	SameLine(250, 0);
	BulletText("Set_Rotation");

	SameLine(360, 0);
	PushItemWidth(50.0f);

	if (InputFloat("##Set_Rotation", &m_fValue))
	{

		if (m_bEvent1)
		{
			//fRotation.x = m_fValue;
			m_pObject->Get_MapObjDesc().fRotValue.x = m_fValue;
		}
		else if (m_bEvent2)
		{
			//fRotation.y = m_fValue;
			m_pObject->Get_MapObjDesc().fRotValue.y = m_fValue;
		}
		else if (m_bEvent3)
		{
			//fRotation.z = m_fValue;
			m_pObject->Get_MapObjDesc().fRotValue.z = m_fValue;
		}


		_float4 fScale{};
		XMStoreFloat4(&fScale, m_pTransform->Get_Scale());

		_vector vPos = m_pTransform->Get_State(CTransform::STATE_POS);

		_vector vResult = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(fRotation.x), XMConvertToRadians(fRotation.y), XMConvertToRadians(fRotation.z));

		_float4x4 fMatrix{};

		XMStoreFloat4x4(&fMatrix, XMMatrixRotationQuaternion(vResult));
		m_pTransform->Set_Matrix(&fMatrix);

		m_pTransform->Set_State(CTransform::STATE_POS, vPos);


	}
	ImGui::PopItemWidth();

	if (SliderFloat("Rotation", &m_fValue, -180.f, 180.f))
	{
		_vector vResult = XMQuaternionRotationRollPitchYaw(XMConvertToRadians(fRotation.x), XMConvertToRadians(fRotation.y), XMConvertToRadians(fRotation.z));

		_vector vPos = m_pTransform->Get_State(CTransform::STATE_POS);
		_float4x4 fMatrix {};

		_float4 fScale{};
		XMStoreFloat4(&fScale, m_pTransform->Get_Scale());
		m_pTransform->SetUp_Scaled(fScale.x, fScale.y, fScale.z);

		XMStoreFloat4x4(&fMatrix, XMMatrixRotationQuaternion(vResult));
		m_pTransform->Set_Matrix(&fMatrix);

		if (m_bEvent1)
		{
			m_pObject->Get_MapObjDesc().fRotValue.x = m_fValue;
		}
		else if (m_bEvent2)
		{
			m_pObject->Get_MapObjDesc().fRotValue.y = m_fValue;
		}
		else if (m_bEvent3)
		{
			m_pObject->Get_MapObjDesc().fRotValue.z = m_fValue;
		}

		m_pTransform->Set_State(CTransform::STATE_POS, vPos);

		// 스케일 조정 빡셈
		/*_float4 fScale{};
		XMStoreFloat4(&fScale, m_pTransform->Get_Scale());

		m_pTransform->SetUp_Scaled(fScale.x, fScale.y, fScale.z);*/
	}
}

void CImGui_Map::Render_TransformPosition()
{
	_float4 fPos{};
	XMStoreFloat4(&fPos, m_pTransform->Get_State(CTransform::STATE_POS));

	Button_Info(fPos);

	_float fValueX, fValueY, fValueZ = {};

	fValueX = fPos.x;
	fValueY = fPos.y;	
	fValueZ = fPos.z;

	//SameLine(250, 0);
	BulletText("Set_Position");

	//SameLine(360, 0);
	PushItemWidth(50.0f);

#pragma region X Input
	Text("x : ");
	SameLine(40, -1);
	if (InputFloat("##x : ", &fValueX) &&
		m_pGameInstance->Key_Down(DIK_RETURN))
	{
		m_pTransform->Set_State(CTransform::STATE_POS,
			XMVectorSet(fValueX, fValueY, fValueZ, 1.f));
	}
	SameLine(100, 0);
#pragma endregion X Input

#pragma region Y Input
	Text("y : ");
	SameLine(130, 0);
	if (InputFloat("##y : ", &fValueY) &&
		m_pGameInstance->Key_Down(DIK_RETURN))
	{
		m_pTransform->Set_State(CTransform::STATE_POS,
			XMVectorSet(fValueX, fValueY, fValueZ, 1.f));
	}
	SameLine(190, 0);
#pragma endregion Y Input

#pragma region Z Input
	Text("z : ");
	SameLine(220, 0);

	if (InputFloat("##z : ", &fValueZ) &&
		m_pGameInstance->Key_Down(DIK_RETURN))
	{
		m_pTransform->Set_State(CTransform::STATE_POS,
			XMVectorSet(fValueX, fValueY, fValueZ, 1.f));
	}

#pragma endregion Z Input

	ImGui::PopItemWidth();


	if (m_bEvent1)
		m_fValue = fValueX;
	else if (m_bEvent2)
		m_fValue = fValueY;
	else if (m_bEvent3)
		m_fValue = fValueZ;

	if (SliderFloat("Position", &m_fValue, m_fValue - 1.f, m_fValue + 1.f))
	{
		if (m_bEvent1)
			m_pTransform->Set_State(CTransform::STATE_POS,
				XMVectorSet(m_fValue, fValueY, fValueZ, 1.f));
		else if (m_bEvent2)
			m_pTransform->Set_State(CTransform::STATE_POS,
				XMVectorSet(fValueX, m_fValue, fValueZ, 1.f));
		else if (m_bEvent3)
			m_pTransform->Set_State(CTransform::STATE_POS,
				XMVectorSet(fValueX, fValueY, m_fValue, 1.f));
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

	//Safe_Release(m_pTransform);
	//Safe_Release(m_pObject);
}
