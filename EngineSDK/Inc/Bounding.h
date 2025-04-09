#pragma once

#include "Collider.h"

/* aabb, obb, sphere 클래스의 부모가 되는 클래스. */

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoudingDescㄴ
	{
		_float3				   vCenter;

		// 바깥에서 받아와서 사용하기 위해 선언
		// 근데 이 데이터가 그대로 남아서 효율 개박살
		COLL_TYPE			   eType;
		_uint				   iOption;				
		_wstring			   strCollTag = {};
		_bool				   bColls = { false };  
	}BOUNDING_DESC;


	typedef struct tagBoundingDescInfo
	{
		COLL_TYPE			   eType;
		_uint				   iOption;			
		_wstring			   strCollTag = {};
		_bool				   bColls = { false }; 
	}BOUNDING_INFO;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	virtual void*			Get_Desc() = 0;
	virtual BOUNDING_INFO	Get_Infos() { return m_tInfo; }

public:
	virtual void Update(_fmatrix WorldMatrix) = 0;
	virtual _bool Intersect(COLL_TYPE eType, CBounding* pTargetBound) = 0;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) = 0;
#endif

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	BOUNDING_INFO				m_tInfo = {};
public:	
	virtual void Free() override;
};

END