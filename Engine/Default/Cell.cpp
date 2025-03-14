#include "Cell.h"

#ifdef _DEBUG
#include "VIBuffer_Cell.h"
#endif

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{pDevice}
    , m_pContext{pContext}
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
    m_iIndex = iIndex;

    memcpy(m_vPoint, pPoints, sizeof(_float3) * POINT_END);

    _vector vLine[LINE_END] = {};
    vLine[LINE_AB] = XMLoadFloat3(&m_vPoint[POINT_B]) - XMLoadFloat3(&m_vPoint[POINT_A]);
    vLine[LINE_BC] = XMLoadFloat3(&m_vPoint[POINT_C]) - XMLoadFloat3(&m_vPoint[POINT_B]);
    vLine[LINE_CA] = XMLoadFloat3(&m_vPoint[POINT_A]) - XMLoadFloat3(&m_vPoint[POINT_C]);

    for (size_t i = 0; i < LINE_END; i++)
    {
        _float3  vNormal = _float3(XMVectorGetZ(vLine[i]) * -1.f, 0.f, XMVectorGetX(vLine[i]));

        XMStoreFloat3(&m_vNormals[i], XMVector3Normalize(XMLoadFloat3(&vNormal)));
    }

    XMStoreFloat4(&m_vPlane,
        XMPlaneFromPoints(
            XMVectorSetW(XMLoadFloat3(&m_vPoint[POINT_A]), 1.f), 
            XMVectorSetW(XMLoadFloat3(&m_vPoint[POINT_B]), 1.f), 
            XMVectorSetW(XMLoadFloat3(&m_vPoint[POINT_C]), 1.f)));


#ifdef _DEBUG
    m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoint);
    if (nullptr == m_pVIBuffer)
        return E_FAIL;
#endif

    return S_OK;
}

_bool CCell::IsCompare(const _float3* pSourPoint, const _float3* pDestPoint)
{
    if (XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_A]), XMLoadFloat3(pSourPoint)))
    {
        if (XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_B]), XMLoadFloat3(pDestPoint)))
            return true;
        if (XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_C]), XMLoadFloat3(pDestPoint)))
            return true;
    }
    if (XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_B]), XMLoadFloat3(pSourPoint)))
    {
        if (XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_C]), XMLoadFloat3(pDestPoint)))
            return true;
        if (XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_A]), XMLoadFloat3(pDestPoint)))
            return true;
    }
    if (XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_C]), XMLoadFloat3(pSourPoint)))
    {
        if (XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_A]), XMLoadFloat3(pDestPoint)))
            return true;
        if (XMVector3Equal(XMLoadFloat3(&m_vPoint[POINT_B]), XMLoadFloat3(pDestPoint)))
            return true;
    }

    return _bool();
}

_bool CCell::IsIn(_fvector vPosition, _int* pNeoghborIndex)
{
    return _bool();
}

_vector CCell::Compute_Height(_fvector vPosition)
{
    return _vector();
}

CCell* CCell::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex)
{
    CCell* pInstance = new CCell(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pPoints, iIndex)))
    {
        MSG_BOX("Failed To Created : CCell");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CCell::Free()
{
    __super::Free();

#ifdef _DEBUG
    Safe_Release(m_pVIBuffer);
#endif
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
