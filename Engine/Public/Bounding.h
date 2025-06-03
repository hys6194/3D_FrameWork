#pragma once

#include "Collider.h"

/* aabb, obb, sphere 클래스의 부모가 되는 클래스. */

BEGIN(Engine)

class CBounding abstract : public CBase
{
public:
	typedef struct tagBoudingInfo
	{
		class CGameObject* pOwner;

		TYPE		eType;
		_wstring	strCollTag;
		_uint		iOption;
	}BOUNDING_INFO;

protected:
	CBounding(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~CBounding() = default;

public:
	TYPE*						Get_Type()					{ return &m_eType; }
	CCollider*					Get_Collider()				{ return m_pOwner; }

	BOUNDING_INFO*				Get_Info()
	{
		return &m_tInfo;
	}

public:
	virtual void				Update(_fmatrix WorldMatrix) = 0;
	virtual _bool				Intersect(TYPE eType, CBounding* pTargetBound) = 0;

#ifdef _DEBUG
public:
	virtual HRESULT				Render(PrimitiveBatch<VertexPositionColor>* pBatch, _fvector vColor) = 0;
#endif

protected:
	ID3D11Device*				m_pDevice		= { nullptr };
	ID3D11DeviceContext*		m_pContext		= { nullptr };

	CCollider*					m_pOwner		= { nullptr };

	class CGameInstance*		m_pGameInstance = { nullptr };


	TYPE						m_eType			= { TYPE_END };
	BOUNDING_INFO				m_tInfo = { nullptr };

public:	
	virtual void Free() override;
};

END