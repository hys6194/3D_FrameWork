#pragma once

#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL CCollider final : public CComponent
{
private:
	CCollider(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CCollider(const CCollider& Prototype);
	virtual ~CCollider() = default;

public:
	class CBounding* Get_Collider()
	{
		return m_pBounding;
	};


public:
	virtual HRESULT							Initialize_Prototype();
	virtual HRESULT							Initialize(void* pArg) override;

public:
	void									Update(_fmatrix WorldMatrix);
	_bool									Intersect(CCollider* pTargetCollider);
	void									Reset() {
		m_isColl = false;
	}

#ifdef _DEBUG
	HRESULT Render();
#endif

private:
	COLL_TYPE								m_eColliderType = { TYPE_END };
	class CBounding*						m_pBounding = { nullptr };

	_bool									m_isColl = { false };

#ifdef _DEBUG
private:
	PrimitiveBatch<VertexPositionColor>*	m_pBatch = {nullptr};
	BasicEffect*							m_pEffect = { nullptr };
	ID3D11InputLayout*						m_pInputLayout = { nullptr };

#endif

public:
	static CCollider*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*						Clone(void* pArg) override;
	virtual void							Free() override;
};

END