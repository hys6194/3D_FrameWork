#pragma once

#include "GameInstance.h"
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL State abstract: public Base
{
protected:
	State(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner, GameInstance* pGameInstance);
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
	virtual void PriorityUpdate_State(_float fTimeDelta) = 0;
	virtual void Update_State(_float fTimeDelta) = 0;
	virtual void LateUpdate_State(_float fTimeDelta) = 0;
	virtual HRESULT Exit_State() = 0;

	virtual void Set_PreAnimation() = 0;
	virtual void Update_Animation(_float fTimeDelta) = 0;
	virtual void Set_CurAnimation() = 0;

protected:
	_uint						m_iState = {};
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	_bool						m_bIsInter = { true };

	GameObject*					m_pOwner = { nullptr };
	GameObject*					m_pAnimOwner = { nullptr };

	GameInstance*				m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END