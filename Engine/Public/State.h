#pragma once

#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL State abstract: public Base
{
protected:
	State(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner);
	virtual ~State() = default;

public:
	void Set_Owner(GameObject* _pOwner)
	{
		m_pOwner = _pOwner;
		Safe_AddRef(m_pOwner);
	};

public:
	GameObject* Get_Owner()
	{
		return m_pOwner;
	};

public:
	virtual HRESULT Enter_State() = 0;
	virtual void Update_State(_float fTimeDelta) = 0;
	virtual _float Exit_State() = 0;

protected:
	_uint						m_iState = {};
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	GameObject*					m_pOwner = { nullptr };


public:
	virtual void Free() override;
};

END