#include "Tool_Manager.h"
#include "GameInstance.h"

#include "ImGui_UI.h"
#include "ImGui_Map.h"
#include "Map_Object.h"
#include "Mesh.h"

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
		if (bNavi && m_pGameInstance->Get_DIMouseState(DIM_LB))
			Create_NaviCells();
		else if (!bNavi && m_pGameInstance->Get_DIMouseState(DIM_LB))
			Picking_Objects();
	}

}

void Tool_Manager::Late_Update(_float fTimeDelta)
{
}

HRESULT Tool_Manager::Render()
{
	return S_OK;
}

void Tool_Manager::Picking_Objects()
{
	// 여기서 해당 오브젝트의 충돌을 확인해야 함
	//_float4 fRayDir;
	//fRayDir = *m_pGameInstance->Get_RayDirCoords();
	//
	//_float4 fRayOrigin;
	//XMStoreFloat4(&fRayOrigin, XMLoadFloat4(m_pGameInstance->Get_CamPosition()));
	//
	//// GameObjectTag에 해당하는 Object를 모은 list
	//list<CGameObject*>* pList = m_pMap->Get_ObjectList();
	//
	//// 리스트가 생성되지 않았으면 return
	//if (nullptr == pList)
	//	return;
	//
	//list<CMap_Object*> test = *reinterpret_cast<list<CMap_Object*>*>(pList);
	//
	//for (auto& iter : test)
	//{
	//	_float4 fCoord;
	//
	//	_bool bTest = iter->Get_ModelCom()->CheckRayColl_Mesh(&fCoord);
	//
	//	if (bTest)
	//		return;
	//
	//}

	int a = 10;
}

void Tool_Manager::Create_NaviCells()
{
	// GameObjectTag에 해당하는 Object를 모은 list
	list<CGameObject*>* pList = m_pMap->Get_ObjectList();

	// 리스트가 생성되지 않았으면 return
	if (nullptr == pList)
		return;

	list<CMap_Object*> test = *reinterpret_cast<list<CMap_Object*>*>(pList);

	for (auto& iter : test)
	{
		_float4 fCoord;

		_bool bTest = iter->Get_ModelCom()->CheckRayColl_Mesh(&fCoord);

		//if (bTes
		//	return;

	}


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

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);

}
