#pragma once

#include "Client_Defines.h"
#include "State.h"


BEGIN(Engine)
class CState;
class CModel;
END

// Base State에서 해야할 것
// 1. 몬스터의 Dead Check
// 2. 피격 판정
// 3. 몬스터의 Look 방향 설정 (1초마다 PlayerPos 받아서 갱신해야함) -> Trace에서 갱신해서 갱신하는 것이 좋아보이긴 함
// Idle에서 다른 스테이트에서 진입했을 때 IsDetected로 플레이어 감지했는지 안했는지 확인?
// SearchState를 만들어서 관리? 이게 맞다 Search_State로 Attack 끝나면 Search로 이동해서 관리하는 게 좋아보임
// 이전 스테이트 값도 필요함


// 야 이거 설계 잘못했나 Update마다 계속 dynamic cast해야하네

BEGIN(Client)

class CMonsterState_Base : public CState
{
protected:
	CMonsterState_Base(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CMonsterState_Base() = default;

public:
	void									Set_AnimIndex(_uint iIndex) { m_iAnimIndex = iIndex;}
	
	// 상태 관련 가상함수
public:
	// CState을(를) 통해 상속됨
	virtual HRESULT							Enter_State()							= 0;
	virtual void							PriorityUpdate_State(_float fTimeDelta);
	virtual	void							Update_State(_float fTimeDelta)			= 0;
	virtual	void							LateUpdate_State(_float fTimeDelta)		= 0;
	virtual	HRESULT							Exit_State()							= 0;

	// 애니메이션 관리 순수 가상 함수
public:
	void									Set_PreAnimation()						= 0;
	void									Update_Animation(_float fTimeDelta)		override;
	void									Set_CurAnimation()						= 0;

protected:
	_vector									Calculate_MonsterDir(_vector vTargetPos);
	_bool									Update_MonsterLook(_float fTimeDelta);
	_float									Update_MonsterTurnSpeed(_float fSpeed = 1.f);
	void									Setting_PlayerInfo();

	HRESULT									Check_Dead(_float fTimeDelta);
	HRESULT									Check_Hit(_float fTimeDelta);


protected:	
	class CModel*							m_pModelCom								= { nullptr };

protected:
	_bool									m_bIsDesd								= { false };
	_bool									m_bAnimEnd								= { false };

	_uint									m_iAnimIndex							= {};
	_uint									m_iPreState								= {};

protected:
	class CMonster*							m_pMonster								= { nullptr };	
	
	// 이 부분도 문제가 될 듯한데?
	class CBody_Ghoul*						m_pBody									= { nullptr };
	class CGameObject*						m_pPlayer 								= { nullptr };

public:
	virtual void							Free()									override;
};

END