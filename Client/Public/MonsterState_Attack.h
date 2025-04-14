#pragma once

#include "Client_Defines.h"
#include "MonsterState_Base.h"

BEGIN(Engine)
class CState;
class CModel;
END

BEGIN(Client)

class CMonsterState_Attack : public CMonsterState_Base
{
private:
	CMonsterState_Attack(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CMonsterState_Attack() = default;


public:
	HRESULT							Enter_State() override;
	void							PriorityUpdate_State(_float fTimeDelta) override;
	void							Update_State(_float fTimeDelta) override;
	void							LateUpdate_State(_float fTimeDelta) override;
	HRESULT							Exit_State() override;

public:
	void							Set_PreAnimation() override;
	void							Update_Animation(_float fTimeDelta) override;
	void							Set_CurAnimation() override;


private:
	void							Regist_PartCollUpdate();
	void							Secede_PartCollUpdate();
private:
	_bool							m_bRegisted = { false };
	_bool							m_bSeceded = { false };

	_float							m_fDistance		= {};
	_float							m_fTotalTime	= {};

public:
	// 몬스터들이 공용으로 사용할 State라서 애니메이션 인덱스를 인자로 받아 출력하게 한다
	static CMonsterState_Attack*	Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex);
	virtual void					Free() override;
};

END