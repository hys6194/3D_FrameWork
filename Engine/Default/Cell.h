#pragma once

#include "Base.h"

BEGIN(Engine)

class CCell : public CBase
{
public:
	enum POINT								{ POINT_A, POINT_B, POINT_C, POINT_END };
	enum LINE								{ LINE_AB, LINE_BC, LINE_CA, LINE_END };

private:
											CCell(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual									~CCell() = default;

public:
	const _float3* Get_Point(POINT ePoint)
	{
		return &m_vPoint[ePoint];
	}

public:
	void Set_Neighbor(LINE eLine, CCell* pNeighbor) 
	{
		m_iNeighbors[eLine] = pNeighbor->m_iIndex;
	}




#ifdef _DEBUG
public:
	HRESULT Render();
#endif

public:
	HRESULT									Initialize(const _float3* pPoints, _int iIndex);

	_bool									IsCompare(const _float3* pSourPoint, const _float3* pDestPoint);
	_bool									IsIn(_fvector vPosition, _int* pNeoghborIndex);
	_vector									Compute_Height(_fvector vPosition);


private:
	ID3D11Device*							m_pDevice					= { nullptr };
	ID3D11DeviceContext*					m_pContext					= { nullptr };

private:
	_float3									m_vPoint[POINT_END];
	_float3									m_vNormals[LINE_END]		= {};
	_int									m_iIndex					= {};
	_int									m_iNeighbors[LINE_END]		= { -1, -1, -1 };

	_float4									m_vPlane					= {};


	// 디버깅 모드일 때만 버퍼를 만드려고 함
#ifdef _DEBUG
private:
	class CVIBuffer_Cell*					m_pVIBuffer					= { };
#endif

public:
	static CCell*							Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _float3* pPoints, _int iIndex);
	virtual void							Free() override;


};

END