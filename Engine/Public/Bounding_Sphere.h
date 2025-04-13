#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_Sphere final : public CBounding
{
public:
	typedef struct tagBoudingSphereDesc
	{
		class CGameObject* pOwner;

		TYPE		eType;
		_wstring	strCollTag;
		_uint		iOption;
		_float		fRadius;
		_float3		vCenter;
		_uint		iAttack; // 아...이거 절대 아닌데
	}BOUNDING_SPHERE_DESC;

private:
	CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_Sphere() = default;

public:
	BoundingSphere*				Get_Desc() {
		return m_pDesc;
	}

public:
	HRESULT						Initialize(const CBounding_Sphere::BOUNDING_SPHERE_DESC* pDesc, class CCollider* pOwner);
	virtual void				Update(_fmatrix WorldMatrix) override;
public:
	virtual _bool				Intersect(TYPE eType, CBounding* pTargetBound) override;

#ifdef _DEBUG
public:
	virtual HRESULT				Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

private:
	BoundingSphere*				m_pLocalDesc = { nullptr };
	BoundingSphere*				m_pDesc = { nullptr };	

public:
	static CBounding_Sphere*	Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding_Sphere::BOUNDING_SPHERE_DESC* pDesc, class CCollider* pOwner);
	virtual void				Free() override;

};

END