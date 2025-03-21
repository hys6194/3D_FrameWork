#pragma once

#include "Base.h"

BEGIN(Engine)

class CCell final : public CBase
{
public:
	enum POINT { POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE { LINE_AB, LINE_BC, LINE_CA, LINE_END };
private:
	CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CCell() = default;

public:
	const _float3* Get_Point(POINT ePoint) {
		return &m_vPoints[ePoint];		
	}
public:
	void Set_Neighbor(LINE eLine, CCell* pNeighbor) {
		m_iNeighbors[eLine] = pNeighbor->m_iIndex;
	}


public:
	HRESULT Initialize(const _float3* pPoints, _int iIndex);

	_bool isCompare(const _float3* pSourPoint, const _float3* pDestPoint);
	_bool isIn(_fvector vPosition, _int* pNeighborIndex);
	_vector Compute_Height(_fvector vPosition);

#ifdef _DEBUG
public:
	HRESULT Render();
#endif



private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };

	_float3					m_vPoints[POINT_END] = {};
	_float3					m_vNormals[LINE_END] = {};
	_int					m_iIndex = {};
	_int					m_iNeighbors[LINE_END] = { -1, -1, -1 };

	_float4					m_vPlane = {};



#ifdef _DEBUG
private:
	class CVIBuffer_Cell* m_pVIBuffer = { };
#endif


public:
	static CCell* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);	
	virtual void Free() override;
};

END