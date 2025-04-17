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

	virtual _bool							Check_Attackable() = 0;
	virtual _bool							Check_Colls() = 0;

protected:
	void									Regist_PartCollUpdate();
	void									Secede_PartCollUpdate();

	void									Check_Patterns();

	void									Regist_CollUpdate(_uint iKeyFrame1, _uint iKeyFrame2);
	void									Secede_CollUpdate(_uint iKeyFrame1, _uint iKeyFrame2);

protected:
	_bool									m_bCool		= { false };
	_bool									m_bAttack	= { false };
	_bool									m_bRegisted = { false };
	_bool									m_bSeceded = { false };

	_float									m_fDistance = {};
	_float									m_fTotalTime = {};
	_float									m_fCoolTime = {};			// 패턴 자체가 가지는 쿨타임
	_float									m_fElapseTime = { 999.f };	// 처음 경과시간은 큰 값으로 시작해 바로 몬스터가 공격할 수 있도록 설정

public:
	virtual void							Free() override;
};

END