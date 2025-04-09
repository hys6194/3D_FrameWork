#pragma once

#include "Collider.h"

/* aabb, obb, sphere 클래스의 부모가 되는 클래스. */

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoudingDesc
	{
		_float3		vCenter;
	}BOUNDING_DESC;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	virtual void*				Get_Desc() = 0;

public:
	virtual void				Update(_fmatrix WorldMatrix) = 0;
	virtual _bool				Intersect(CCollider::TYPE eType, CBounding* pTargetBound) = 0;

#ifdef _DEBUG
public:
	virtual HRESULT				Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) = 0;
#endif

protected:
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	class CGameInstance*		m_pGameInstance = { nullptr };

public:
	virtual void				Free() override;
};

END