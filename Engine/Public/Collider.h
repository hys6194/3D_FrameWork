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
	virtual HRESULT							Initialize_Prototype();
	virtual HRESULT							Initialize_Prototype(TYPE eType);
	virtual HRESULT							Initialize(void* pArg) override;

public:
	class CBounding*						Get_Bounder()
	{
		return m_pBounding;
	}

	_bool									Is_Coll()
	{
		return m_isColl;
	}

public:
	void									Set_Coll(_bool bToogle)	{ m_isColl = bToogle; }

// 이 함수들은 매니저로 옮겨야 할 수도 있다
public:
	void									Update(_fmatrix WorldMatrix);
	_bool									Intersect(CCollider* pTargetCollider);
	void									Reset() {
		m_isColl = false;
	}

#ifdef _DEBUG
	virtual HRESULT							Render();
#endif

private:
	TYPE									m_eColliderType = { TYPE_END };
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
	static CCollider*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, TYPE eType);
	virtual CComponent*						Clone(void* pArg) override;
	virtual void							Free() override;
};

END