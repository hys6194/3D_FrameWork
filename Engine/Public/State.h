#pragma once

#include "GameInstance.h"
#include "GameObject.h"
#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL State abstract: public CBase
{
protected:
	State(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner, CGameInstance* pGameInstance);
	virtual ~State() = default;

public:
	void Set_Owner(CGameObject* _pOwner)
	{
		m_pOwner = _pOwner;
		Safe_AddRef(m_pOwner);
	};

public:
	CGameObject* Get_Owner()
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
	ID3D11Device*				m_pDevice = { nullptr };
	ID3D11DeviceContext*		m_pContext = { nullptr };

	_bool						m_bIsInter = { true };

	CGameObject*					m_pOwner = { nullptr };
	CGameObject*					m_pAnimOwner = { nullptr };

	CGameInstance*				m_pGameInstance = { nullptr };

public:
	virtual void Free() override;
};

END