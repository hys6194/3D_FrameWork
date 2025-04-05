#pragma once

#include "Client_Defines.h"
#include "MonsterState_Base.h"

BEGIN(Engine)
class CState;
class CModel;
END

BEGIN(Client)

class CMonsterState_Hit : public CMonsterState_Base
{
private:
	CMonsterState_Hit(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CMonsterState_Hit() = default;


public:
	virtual HRESULT					Enter_State() override;
	virtual void					PriorityUpdate_State(_float fTimeDelta) override;
	virtual void					Update_State(_float fTimeDelta) override;
	virtual void					LateUpdate_State(_float fTimeDelta) override;
	virtual HRESULT					Exit_State() override;

public:
	virtual void					Set_PreAnimation() override;
	virtual void					Update_Animation(_float fTimeDelta) override;
	virtual void					Set_CurAnimation() override;

private:
	_float							m_fDistance = { 15.f };

public:
	// 몬스터들이 공용으로 사용할 State라서 애니메이션 인덱스를 인자로 받아 출력하게 한다
	static CMonsterState_Hit*		Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex);
	virtual void					Free() override;
};

END