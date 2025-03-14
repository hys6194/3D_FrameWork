#include "Navigation.h"
#include "Cell.h"

#include "Shader.h"
#include "GameInstance.h"


const _float4x4* CNavigation::m_pWorldMatrix = { nullptr };

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent{pDevice, pContext}
{
}

CNavigation::CNavigation(const CNavigation& Prototype)
	: CComponent{ Prototype }
	, m_pShader{ Prototype.m_pShader }
	, m_vecCell{ Prototype.m_vecCell }
{
	for (auto& pCell : m_vecCell)
		Safe_AddRef(pCell);

	Safe_AddRef(m_pShader);
}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFilePath)
{
	_ulong			dwByte = {};
	HANDLE			hFile = CreateFile(pNavigationDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);

	if (0 == hFile)
		return E_FAIL;

	_float3		vPoint[3] = {};

	while (true)
	{
		ReadFile(hFile, vPoint, sizeof(_float3) * 3, &dwByte, nullptr);
		if (0 == dwByte)
			break;

		CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoint, m_vecCell.size());
		if (nullptr == pCell)
			return E_FAIL;

		m_vecCell.push_back(pCell);

	}


	if (FAILED(Ready_Neighbor()))
		return E_FAIL;


#ifdef _DEBUG
	m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Cell.hlsl"),
		VTXPOS::ElementDesc, VTXPOS::iNumElements);
#endif


	return S_OK;
}

HRESULT CNavigation::Initialize(void* pArg)
{
	if (nullptr != pArg)
	{
		NAVIGATION_DESC* pDesc = static_cast<NAVIGATION_DESC*>(pArg);
		m_iCurrentCellIndex = pDesc->iCellIndex;

		if (nullptr != pDesc->pWorldMatrix)
			m_pWorldMatrix = pDesc->pWorldMatrix;
	}


	return S_OK;
}

_bool CNavigation::IsMove(_fvector vPosition)
{
	_int        iNeighborIndex = { -1 };

	// 플레이어가 안으로 들어왔을 경우
	if (true == m_vecCell[m_iCurrentCellIndex]->IsIn(vPosition, &iNeighborIndex))
		return true;
	
	// 인덱스 바깥으로 나가게 된 경우
	else
	{
		// 인덱스가 있을 경우
		if (-1 != iNeighborIndex)
		{
			while (true)
			{
				// 여러 인덱스가 모이는 곳을 순회하다 잘못 읽어들이는 경우
				if (-1 == iNeighborIndex)
					return false;
				 
				// 이웃에 있는 선 바깥으로 플레이어가 존재한다면
				if (true == m_vecCell[iNeighborIndex]->IsIn(vPosition, &iNeighborIndex))
				{
					// 인덱스 갱신
					m_iCurrentCellIndex = iNeighborIndex;
					break;
				}
			}
			return true;
		}
		else
			return false;

	}
}

_vector CNavigation::Compute_Height(_fvector vPosition)
{
	return m_vecCell[m_iCurrentCellIndex]->Compute_Height(vPosition);
}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
	if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", m_pWorldMatrix)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShader,CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShader,CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	m_pShader->Begin(0);

	for (auto& pCell : m_vecCell)
		pCell->Render();

	return S_OK;
}

#endif

HRESULT CNavigation::Ready_Neighbor()
{
	for (auto& pSourCell : m_vecCell)
	{
		for (auto& pDestCell : m_vecCell)
		{
			// 하나의 점이 무조건 맞아야 통과
			if (pSourCell == pDestCell)
				continue;

			
			if (true == pDestCell->IsCompare(pSourCell->Get_Point(CCell::POINT_A), pDestCell->Get_Point(CCell::POINT_B)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
			}

			else if (true == pDestCell->IsCompare(pSourCell->Get_Point(CCell::POINT_B), pDestCell->Get_Point(CCell::POINT_C)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
			}

			else if (true == pDestCell->IsCompare(pSourCell->Get_Point(CCell::POINT_C), pDestCell->Get_Point(CCell::POINT_A)))
			{
				pSourCell->Set_Neighbor(CCell::LINE_CA, pDestCell);
			}
		}

	}


	return S_OK;
}

CNavigation* CNavigation::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pNavigationDataFilePath)
{
	CNavigation* pInstance = new CNavigation(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pNavigationDataFilePath)))
	{
		MSG_BOX("Failed To Created : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CNavigation::Clone(void* pArg)
{
	CNavigation* pInstance = new CNavigation(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CNavigation");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CNavigation::Free()
{
	__super::Free();

	Safe_Release(m_pShader);

	for (auto& iter : m_vecCell)
		Safe_Release(iter);

	m_vecCell.clear();

}
