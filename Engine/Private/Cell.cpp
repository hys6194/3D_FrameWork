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


        // 선분으로 부터 직교하는 법선 벡터
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
    // 지나가고자 하는 인덱스가 이전의 이웃인지 확인하는 함수
    // 
    // A와 인자로 받아온 녀석이 같은 정점인지 부터 확인
    // 그 뒤, 둘을 확인하고 2개 이상 일치한다면, true를 리턴해서 지나갈 수있게 bool 변수를 전달
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
}

_bool CCell::IsIn(_fvector vPosition, _int* pNeighborIndex)
{

    // 플레이어의 위치와 한 점으로부터 방향벡터를 구하고, 내적의 결과가 양수면 바깥으로 인식
    // 노트로 그려보면서 정리해보셈
    for (size_t i = 0; i < LINE_END; i++)
    {
        _vector	vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoint[i]));

        if (0 < XMVectorGetX(XMVector3Dot(vDir, XMLoadFloat3(&m_vNormals[i]))))
        {
            *pNeighborIndex = m_iNeighbors[i];
            return false;
        }
    }

    return true;
}

_vector CCell::Compute_Height(_fvector vPosition)
{
    /*ax + by + cz + d = 0
    y = (-ax - cz - d) / b*/
    // 두 평면의 기울기의 공식을 통해 y 좌표를 올린다
    return XMVectorSet(
        XMVectorGetX(vPosition),
        (-m_vPlane.x * XMVectorGetX(vPosition) - m_vPlane.z * XMVectorGetZ(vPosition) - m_vPlane.w) / m_vPlane.y,
        XMVectorGetZ(vPosition),
        1.f);
}

#ifdef _DEBUG
HRESULT CCell::Render()
{
    m_pVIBuffer->Bind_Input_Assembler();

    m_pVIBuffer->Render();

    return S_OK;
}
#endif

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
