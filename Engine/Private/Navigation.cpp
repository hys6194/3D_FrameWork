#include "Navigation.h"
#include "Cell.h"

#include "GameInstance.h"

#include "Shader.h"

const _float4x4* CNavigation::m_pWorldMatrix = { nullptr };

CNavigation::CNavigation(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CComponent { pDevice, pContext }
{
}

CNavigation::CNavigation(const CNavigation& Prototype)
	: CComponent { Prototype }
    , m_pShader { Prototype.m_pShader }
    , m_Cells { Prototype.m_Cells }
{
    for (auto& pCell : m_Cells)
        Safe_AddRef(pCell);

    Safe_AddRef(m_pShader);
}

HRESULT CNavigation::Initialize_Prototype(const _tchar* pNavigationDataFilePath)
{
    _ulong          dwByte = {};
    HANDLE          hFile = CreateFile(pNavigationDataFilePath, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
    if (0 == hFile)
        return E_FAIL;

    _float3     vPoints[3] = {};

    while (true)
    {
        ReadFile(hFile, vPoints, sizeof(_float3) * 3, &dwByte, nullptr);
        if (0 == dwByte)
            break;

        CCell* pCell = CCell::Create(m_pDevice, m_pContext, vPoints, m_Cells.size());
        if (nullptr == pCell)
            return E_FAIL;

        m_Cells.push_back(pCell);
    }

    if (FAILED(Ready_Neighbors()))
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


_bool CNavigation::isMove(_fvector vPosition)
{
    _int        iNeighborIndex = { -1 };

    if (true == m_Cells[m_iCurrentCellIndex]->isIn(vPosition, &iNeighborIndex))
        return true;

    else
    {
        if (-1 != iNeighborIndex)
        {
            while (true)
            {
                if (-1 == iNeighborIndex)
                    return false;

                if (true == m_Cells[iNeighborIndex]->isIn(vPosition, &iNeighborIndex))
                {
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
    return m_Cells[m_iCurrentCellIndex]->Compute_Height(vPosition);    
}

//
//_bool CNavigation::isMove(_fvector vPosition)
//{
//    if (true == m_Cells[m_iCurrentCellIndex]->isIn(vPosition))
//        return true;
//
//    else
//    {
//        if (나간방향에 이웃이 있었냐 ? )
//        {
//            m_iCurrentCellIndex = 이웃의 인덱스;
//            return true;
//        }
//        else
//        {
//            return false;
//        }
//
//    }
//
//    return _bool();
//}

#ifdef _DEBUG
HRESULT CNavigation::Render()
{
    if (FAILED(m_pShader->Bind_Matrix("g_WorldMatrix", m_pWorldMatrix)))
        return E_FAIL;

    if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShader, CPipeLine::D3DTS_VIEW)))
        return E_FAIL;
    if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShader, CPipeLine::D3DTS_PROJ)))
        return E_FAIL;

    m_pShader->Begin(1);

    for (auto& pCell : m_Cells)
        pCell->Render();

    return S_OK;
}

#endif

HRESULT CNavigation::Ready_Neighbors()
{
    for (auto& pSourCell : m_Cells)
    {
        for (auto& pDestCell : m_Cells)
        {
            if (pSourCell == pDestCell)
                continue;

            if(true == pDestCell->isCompare(pSourCell->Get_Point(CCell::POINT_A), pSourCell->Get_Point(CCell::POINT_B)))
            {
                pSourCell->Set_Neighbor(CCell::LINE_AB, pDestCell);
            }

            else if (true == pDestCell->isCompare(pSourCell->Get_Point(CCell::POINT_B), pSourCell->Get_Point(CCell::POINT_C)))
            {
                pSourCell->Set_Neighbor(CCell::LINE_BC, pDestCell);
            }

            else if (true == pDestCell->isCompare(pSourCell->Get_Point(CCell::POINT_C), pSourCell->Get_Point(CCell::POINT_A)))
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

	for (auto& pCell : m_Cells)
		Safe_Release(pCell);

	m_Cells.clear();
}
