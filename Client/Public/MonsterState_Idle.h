#pragma once

#include "Client_Defines.h"
#include "MonsterState_Base.h"

BEGIN(Engine)
class CState;
class CModel;
END

BEGIN(Client)

class CMonsterState_Idle : public CMonsterState_Base
{
	// 진짜 뭐가 문제인걸까
	// 아무리 생각해봐도 이유를 모르겠다
	// 어쨰서 애니메이션이 끝날 때 공유되는 현상이 일어나는지를 모르겠다

private:
	CMonsterState_Idle(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CMonsterState_Idle() = default;


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


public:
	// 몬스터들이 공용으로 사용할 State라서 애니메이션 인덱스를 인자로 받아 출력하게 한다
	static CMonsterState_Idle*		Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex);
	virtual void					Free() override;
};

END