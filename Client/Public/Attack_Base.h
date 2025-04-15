#pragma once

#include "Client_Defines.h"
#include "MonsterState_Base.h"

BEGIN(Client)

class CAttack_Base : public CMonsterState_Base
{
protected:
	CAttack_Base(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CAttack_Base() = default;

public:
	virtual HRESULT							Enter_State() = 0;
	virtual void							PriorityUpdate_State(_float fTimeDelta) = 0;
	virtual void							Update_State(_float fTimeDelta) = 0;
	virtual void							LateUpdate_State(_float fTimeDelta) = 0;
	virtual HRESULT							Exit_State() = 0;

	virtual void							Set_PreAnimation() = 0 ;
	virtual void							Update_Animation(_float fTimeDelta) override ;
	virtual void							Set_CurAnimation() = 0;
	virtual void							Update_CoolTime(_float fTimeDelta) = 0;

protected:
	void									Regist_PartCollUpdate();
	void									Secede_PartCollUpdate();


protected:
	_bool									m_bCool		= { false };
	_bool									m_bAttack	= { false };

	_float									m_fCoolTime = {};

public:
	virtual void							Free() override;
};

END