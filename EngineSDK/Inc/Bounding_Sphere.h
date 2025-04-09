#pragma once

#include "Bounding.h"

BEGIN(Engine)

class CBounding_Sphere final : public CBounding
{
public:
	typedef struct tagBoudingSphereDesc : public CBounding::BOUNDING_DESC
	{
		_float		fRadius;
	}BOUNDING_SPHERE_DESC;

private:
	CBounding_Sphere(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding_Sphere() = default;

public:
	virtual void* Get_Desc() {
		return m_pDesc;
	}

public:
	HRESULT Initialize(const CBounding::BOUNDING_DESC* pDesc);
	virtual void Update(_fmatrix WorldMatrix) override;
public:
	virtual _bool Intersect(COLL_TYPE eType, CBounding* pTargetBound) override;

#ifdef _DEBUG
public:
	virtual HRESULT Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) override;
#endif

private:
	BoundingSphere*				m_pLocalDesc = { nullptr };
	BoundingSphere*				m_pDesc = { nullptr };
	

public:
	static CBounding_Sphere* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const CBounding::BOUNDING_DESC* pDesc);
	virtual void Free() override;

};

END