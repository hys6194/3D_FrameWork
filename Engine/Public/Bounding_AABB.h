#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_AABB final : public CBounding
{
public:
	typedef struct tagBoudingAABBDesc
	{
		class CGameObject* pOwner;
		TYPE		eType;
		_wstring	strCollTag;
		_uint		iOption;

		_float3		vCenter;
		_float3		vExtents;
	}BOUNDING_AABB_DESC;

	typedef struct tagBoudingAABBInfo
	{
		class CGameObject* pOwner;
		TYPE		eType;
		_wstring	strCollTag;
		_uint		iOption;
	}BOUNDING_AABB_INFO;

private:
	CBounding_AABB(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_AABB() = default;

public:
	BoundingBox*					Get_Desc() {
		return m_pDesc;
	}

	BOUNDING_AABB_INFO*				Get_Info()
	{
		return &m_tInfo;
	}

public:
	HRESULT							Initialize(const CBounding_AABB::BOUNDING_AABB_DESC* pDesc, class CCollider* pOwner);
	virtual void					Update(_fmatrix WorldMatrix) override;

public:
	virtual _bool					Intersect(TYPE eType, CBounding* pTargetBound) override;

#ifdef _DEBUG
public:
	virtual HRESULT					Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

private:
	BoundingBox*					m_pLocalDesc = { nullptr };
	BoundingBox*					m_pDesc = { nullptr };

	BOUNDING_AABB_INFO				m_tInfo;

private:
	_bool							Intersect_AABB(CBounding_AABB* pTargetBound);
	_float3							Compute_Min();
	_float3							Compute_Max();

	
	

public:
	static CBounding_AABB*			Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding_AABB::BOUNDING_AABB_DESC* pDesc, class CCollider* pOwner);
	virtual void					Free() override;

};

END