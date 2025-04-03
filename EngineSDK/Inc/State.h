#pragma once

#include "GameInstance.h"
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL CState abstract: public CBase
{
protected:
	CState(CGameObject* pOwner, CGameObject* pAnimOwner, CGameInstance* pGameInstance);
	virtual ~CState() = default;

public:
	void									Set_Owner(CGameObject* _pOwner)
	{
		m_pOwner = _pOwner;
		Safe_AddRef(m_pOwner);
	};

public:
	CGameObject*							Get_Owner()
	{
		return m_pOwner;
	};

public:
	virtual HRESULT							Enter_State() = 0;
	virtual void							PriorityUpdate_State(_float fTimeDelta) = 0;
	virtual void							Update_State(_float fTimeDelta) = 0;
	virtual void							LateUpdate_State(_float fTimeDelta) = 0;
	virtual HRESULT							Exit_State() = 0;

	virtual void							Set_PreAnimation() = 0;
	virtual void							Update_Animation(_float fTimeDelta) = 0;
	virtual void							Set_CurAnimation() = 0;

protected:
	_bool									m_bIsInter = { true };
	_uint 									m_iState = {};

	CGameObject*							m_pOwner = { nullptr };
	CGameObject*							m_pAnimOwner = { nullptr };

	CGameInstance*							m_pGameInstance = { nullptr };

public:
	virtual void							Free() override;
};

END