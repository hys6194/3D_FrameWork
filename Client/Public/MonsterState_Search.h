#pragma once

#include "Client_Defines.h"
#include "MonsterState_Base.h"

BEGIN(Engine)
class CState;
class CModel;
END

BEGIN(Client)

class CMonsterState_Search : public CMonsterState_Base
{
private:
	CMonsterState_Search(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CMonsterState_Search() = default;


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
	_float							m_fDistance = { 15.f };

	_bool							m_bCheck = { false };

public:
	// 몬스터들이 공용으로 사용할 State라서 애니메이션 인덱스를 인자로 받아 출력하게 한다
	static CMonsterState_Search*	Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex);
	virtual void					Free() override;
};

END