#pragma once

#include "Collider.h"

/* aabb, obb, sphere 클래스의 부모가 되는 클래스. */

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoudingDesc
	{
		CCollider::TYPE        eType;
		_uint				   iOption;			// 근데 이거 CollOption을 클라에게 어떻게 알게 하지?
		_wstring			   strCollTag = {};
		_float3				   vCenter;
		_bool				   bColls = { false }; // 기본값으로 false를 주고 false 인 녀석들은 충돌매니저에서 제외
	}BOUNDING_DESC;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	virtual void* Get_Desc() = 0;
	virtual CCollider::TYPE       Get_Type()
	{
		return m_eType;
	}

public:
	virtual void Update(_fmatrix WorldMatrix) = 0;
	virtual _bool Intersect(CCollider::TYPE eType, CBounding* pTargetBound) = 0;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) = 0;
#endif

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	CCollider::TYPE				m_eType = { CCollider::TYPE_END };
public:	
	virtual void Free() override;
};

END