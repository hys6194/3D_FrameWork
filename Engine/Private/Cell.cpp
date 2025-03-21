#include "Cell.h"

#ifdef _DEBUG
#include "VIBuffer_Cell.h"
#endif

CCell::CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext { pContext }

{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CCell::Initialize(const _float3* pPoints, _int iIndex)
{
	m_iIndex = iIndex;

	memcpy(m_vPoints, pPoints, sizeof(_float3) * POINT_END);

	_vector		vLines[LINE_END] = {};
	vLines[LINE_AB] = XMLoadFloat3(&m_vPoints[POINT_B]) - XMLoadFloat3(&m_vPoints[POINT_A]);
	vLines[LINE_BC] = XMLoadFloat3(&m_vPoints[POINT_C]) - XMLoadFloat3(&m_vPoints[POINT_B]);
	vLines[LINE_CA] = XMLoadFloat3(&m_vPoints[POINT_A]) - XMLoadFloat3(&m_vPoints[POINT_C]);

	for (size_t i = 0; i < LINE_END; i++)	
	{
		_float3		vNormal = _float3(XMVectorGetZ(vLines[i]) * -1.f, 0.f, XMVectorGetX(vLines[i]));

		XMStoreFloat3(&m_vNormals[i], XMVector3Normalize(XMLoadFloat3(&vNormal)));
	}

	XMStoreFloat4(&m_vPlane, 
		XMPlaneFromPoints(XMVectorSetW(XMLoadFloat3(&m_vPoints[POINT_A]), 1.f), XMVectorSetW(XMLoadFloat3(&m_vPoints[POINT_B]), 1.f), XMVectorSetW(XMLoadFloat3(&m_vPoints[POINT_C]), 1.f)));	

#ifdef _DEBUG
	m_pVIBuffer = CVIBuffer_Cell::Create(m_pDevice, m_pContext, m_vPoints);
	if (nullptr == m_pVIBuffer)
		return E_FAIL;
#endif
	
	return S_OK;
}

_bool CCell::isCompare(const _float3* pSourPoint, const _float3* pDestPoint)
{
	///* 두 벡터의 멤버 세개가 모두 같냐? */
	//XMVector3Equal();

	///* 두 벡터의 멤버 세개가 각각 같냐 안같냐? */
	///* (1, 1, 1, 1) (1, 0, 0, 1) */
	///* 1, 0, 0, 1*/
	//XMVectorEqual();

	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDestPoint)))
			return true;
	}
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDestPoint)))
			return true;
	}
	if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_C]), XMLoadFloat3(pSourPoint)))
	{
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_A]), XMLoadFloat3(pDestPoint)))
			return true;
		if (XMVector3Equal(XMLoadFloat3(&m_vPoints[POINT_B]), XMLoadFloat3(pDestPoint)))
			return true;
	}

	return false;
}

_bool CCell::isIn(_fvector vPosition, _int* pNeighborIndex)
{
	for (size_t i = 0; i < LINE_END; i++)
	{
		_vector	vDir = XMVector3Normalize(vPosition - XMLoadFloat3(&m_vPoints[i]));

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
