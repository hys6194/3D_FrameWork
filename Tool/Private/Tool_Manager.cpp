#include "Tool_Manager.h"
#include "GameInstance.h"

#include "ImGui_UI.h"
#include "ImGui_Map.h"
#include "Map_Object.h"
#include "Mesh.h"
#include "Cell_Guide.h"
#include "Tool_FreeCam.h"

Tool_Manager::Tool_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ pGameInstance }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}

HRESULT Tool_Manager::Initialize()
{
	m_pUI = CImGui_UI::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pUI, E_FAIL);

	m_pMap = CImGui_Map::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pMap, E_FAIL);

	m_pCell = CCell_Guide::Create(m_pDevice, m_pContext);
	NULL_CHECK_RETURN(m_pCell, E_FAIL);

	return S_OK;
}

void Tool_Manager::Update(_float fTimeDelta)
{
	m_pUI->Update(fTimeDelta);
	m_pMap->Update(fTimeDelta);

	_bool bNavi = m_pMap->Is_Navi();

	_float4 fPos;
	XMStoreFloat4(&fPos, *m_pGameInstance->Get_MouseWindowPosition());

	// 마우스가 뷰포트 내에 있을 때에만 실행
	if (fabs(fPos.x) < 1.f && fabs(fPos.y) < 1.f)
	{
		//내비 버튼 눌려지면
		if (bNavi)
		{
			Create_NaviCells();
			m_pCell->Update(fTimeDelta, m_vCellCoord);
		}
		else
		{
			Picking_Objects();
			m_pCell->Delete_VIBuffer();
		}
			
	}

	

	}

void Tool_Manager::Late_Update(_float fTimeDelta)
{
	m_pCell->Late_Update(1.f);
	/*if (m_pCell->Is_Empty() || 
		m_pCell->Is_Null())*/

	//세이브를 여기서 처리
	if (m_pMap->Is_Save() == true)
	{
		m_pCell->Save_Data();
		m_pMap->Toogle_Save();
	}

	if (m_pMap->Is_Load() == true)
	{
		m_pCell->Load_Data();
		m_pMap->Toogle_Load();
	}

	m_pMap->Late_Update(fTimeDelta);
}

HRESULT Tool_Manager::Render()
{
	return S_OK;
}

void Tool_Manager::Picking_Objects()
{
	// 여기서 메쉬 충돌 해서 해당 오브젝트를 가져와야 함
	list<CGameObject*>* pList = m_pGameInstance->Get_GameObjectList(LEVEL_TOOL, TEXT("Layer_Objcet"));;

	// 리스트가 생성되지 않았으면 return
	if (nullptr == pList)
		return;


	if (m_pGameInstance->Mouse_Down(DIM_LB))
	{
		vector<_float4> vecCoord = *m_pGameInstance->Get_RayCoords();

		_vector vRayOrigin = XMLoadFloat4(&vecCoord[0]);
		_vector vRayDir = XMLoadFloat4(&vecCoord[1]);
		_float fDistance{};

		// 이게 훨씬 나음
		list<CMap_Object*> listObjects = *reinterpret_cast<list<CMap_Object*>*>(pList);

		for (auto iter : listObjects)
		{
			// 해당 메쉬의 월드 정점
			CMap_Object* pObject = dynamic_cast<CMap_Object*>(iter);
			_float4 fCoord;
			
			_matrix matWorld = XMLoadFloat4x4(pObject->Get_Transform()->Get_WorldMatrix_Ptr());

			_vector vScale, vRotation, vTranslation;
			XMMatrixDecompose(&vScale, &vRotation, &vTranslation, matWorld);

			//_bool bColl = iter->Get_ModelCom()->CheckRayColl_Mesh(vRayOrigin, vRayDir, &fDistance, &fCoord);
			_bool bColl = pObject->Get_ModelCom()->
				CheckRayColl_Mesh(vRayOrigin, vRayDir, &fDistance, &fCoord, vScale, vRotation, vTranslation);

			if (bColl)
			{
				m_pMap->Set_Select(true);
				m_pMap->Set_TransformInfo(pObject);
			
			}


		}
	}
}

void Tool_Manager::Create_NaviCells()
{
	// GameObjectTag에 해당하는 Object를 모은 list
	list<CGameObject*>* pList = m_pMap->Get_ObjectList();

	// 리스트가 생성되지 않았으면 return
	if (nullptr == pList)
		return;

	vector<_float4> vecCoord = *m_pGameInstance->Get_RayCoords();

	_vector vRayOrigin = XMLoadFloat4(&vecCoord[0]);
	_vector vRayDir = XMLoadFloat4(&vecCoord[1]);
	_float fDistance{};

	// 정점 보정 끝나면
	if (!m_pCell->Get_Modify() || 
		m_pCell->Is_Null())
	{
		// 그리기용 삼각형 생성
		m_pCell->Clone_VIBuffer();
	}

	list<CMap_Object*> listObjects = *reinterpret_cast<list<CMap_Object*>*>(pList);

	for (auto& iter : listObjects)
	{
		// 해당 메쉬의 월드 정점
		_float4 fCoord;
		_bool bColl;

		_matrix matWorld = XMLoadFloat4x4(iter->Get_Transform()->Get_WorldMatrix_Ptr());

		_vector vScale, vRotation, vTranslation;
		XMMatrixDecompose(&vScale, &vRotation, &vTranslation, matWorld);

		// 어떤 오브젝트의 한 면에 충돌하게 되었다면
		// True 반환하면서 모든 메시 충돌 찾는것을 
		if(nullptr == m_pMapObject)
		{
			bColl = iter->Get_ModelCom()->CheckRayColl_Mesh(vRayOrigin, vRayDir, &fDistance, &fCoord, vScale, vRotation, vTranslation);

			if (bColl)
				m_pMapObject = iter;
		}

		if(nullptr != m_pMapObject)
		{
			m_pMapObject = iter;

			// 해당 오브젝트를 순회해서 점을 찾는다
			_bool bTest = m_pMapObject->Get_ModelCom()->
				DotPoint_InMesh(vRayOrigin, vRayDir, &fDistance, &fCoord, vScale, vRotation, vTranslation);

			if (bTest)
			{
				//최소 거리를 가져와서 좌표 가져오기
				m_vCellCoord = vRayOrigin + vRayDir * fDistance;

				return;
			}
			else
			{
				// 다시 오브젝트를 찾게함
				m_pMapObject = nullptr;
			}

		}

	}

	m_bColl = false;
}

void Tool_Manager::Move_CamPos(CMap_Object* pObject)
{
	// 오브젝트 클릭하면 해당 오브젝트로 이동
	_vector vPos = pObject->Get_Transform()->Get_State(CTransform::STATE_POS);

	_float4 fPos{};

	XMStoreFloat4(&fPos, vPos);

	fPos.x = fPos.x * pObject->Get_Transform()->Update_Scale().x;
	fPos.y = (fPos.y + 30) * pObject->Get_Transform()->Update_Scale().x;
	fPos.z = (fPos.z - 30) * pObject->Get_Transform()->Update_Scale().x;

	m_pGameInstance->Find_GameObject(
		LEVEL_TOOL,
		TEXT("Layer_Camera"),
		TEXT("GameObject_Camera_Free"))->
		Get_Transform()->Set_State(CTransform::STATE_POS, XMLoadFloat4(&fPos));

}

Tool_Manager* Tool_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameInstance* pGameInstance)
{
	Tool_Manager* pInstance = new Tool_Manager(pDevice, pContext, pGameInstance);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Tool_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Tool_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pUI);
	Safe_Release(m_pMap);
	Safe_Release(m_pCell);

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

}
