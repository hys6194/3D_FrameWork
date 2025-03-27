#include "Tool_Manager.h"
#include "GameInstance.h"

#include "ImGui_UI.h"
#include "ImGui_Map.h"
#include "Map_Object.h"
#include "Mesh.h"
#include "Cell_Guide.h"

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
			Picking_Objects();
	}

	

}

void Tool_Manager::Late_Update(_float fTimeDelta)
{
	if (m_pCell->Get_Modify() && !m_pCell->Is_Empty())
		m_pCell->Late_Update(1.f);

	//m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, 
	//	m_pCell->Late_Update);
}

HRESULT Tool_Manager::Render()
{
	return S_OK;
}

void Tool_Manager::Picking_Objects()
{


	int a = 10;
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

	// 정점 수정중이 아니라면 Clone하여 만들기
	// 추후 수정해야 할 거임
	if (!m_pCell->Get_Modify())
	{
		m_pCell->Clone_VIBuffer();
	}

	list<CMap_Object*> test = *reinterpret_cast<list<CMap_Object*>*>(pList);

	for (auto& iter : test)
	{
		// 해당 메쉬의 월드 정점
		_float4 fCoord;

		_bool bTest = iter->Get_ModelCom()->CheckRayColl_Mesh(vRayOrigin, vRayDir, &fDistance, &fCoord);

		// 여기에서 여러 메쉬를 비교?
		// 일단 모든 메시를 전부 비교하자 내 머리로는 이게 한계고 시간도 부족하다
		//m_fCurDistance = fDistance;

		/*이전 거리와 최근 거리 비교, 최근 거리가 더 짧다면 이전 거리에 대입
		  오브젝트 순회하면서 제일 짧은 거리를 반환*/
		// 더 간단한 로직이 생각이 안나
		if (bTest)
		{
			if (!m_bTest)
			{
				m_fDistance = fDistance;
				m_vCellCoord = vRayOrigin + vRayDir * m_fDistance;
				m_bTest = true;
			}
			else if(m_bTest && fDistance < m_fDistance)
			{ 
				m_fDistance = fDistance;
				m_vCellCoord = vRayOrigin + vRayDir * m_fDistance;
			}
		}

		//TCHAR debugMessage[256];
		//_stprintf_s(debugMessage, _T("fDistance: %.6f\n"),
		//	m_fDistance);
		//OutputDebugString(debugMessage);

		int a = 10;
	}



	m_bTest = false;
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
