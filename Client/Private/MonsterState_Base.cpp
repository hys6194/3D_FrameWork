#include "MonsterState_Base.h"

#include "Monster.h"
#include "Body_Ghoul.h"

CMonsterState_Base::CMonsterState_Base(CGameObject* pOwner, CGameObject* pAnimOwner)
	: CState{ pOwner, pAnimOwner, m_pGameInstance }
{
	// 이래도 되려나	
	m_pMonster  = dynamic_cast<CMonster*>(m_pOwner);
	m_pBody     = dynamic_cast<CBody_Monster*>(m_pAnimOwner);
	m_pModelCom = m_pBody->Get_Model();
}

HRESULT CMonsterState_Base::Enter_State()
{
	return S_OK;
}

void CMonsterState_Base::PriorityUpdate_State(_float fTimeDelta)
{
	//구조가 바뀌었다
}

void CMonsterState_Base::Update_Animation(_float fTimeDelta)
{
	// 가상함수로 기본적인 애니메이션은 이걸로 돌리고 루트애님이나 그런거 들어간건 override해서 쓰기
	if (0 != m_pModelCom->Get_PreAnimIndex()
		&& m_pModelCom->Get_Interpolate())
		m_pModelCom->Interpolate_Animation(0.2f);
	else
		m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta);
}

_vector CMonsterState_Base::Calculate_MonsterDir(_vector vTargetPos)
{
	// 상속시켜서 다른 함수에서도 사용하게 하자 쓰기 편하게
	return XMVector4Normalize(XMVectorSetY(XMVectorSubtract(vTargetPos, m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS)), 0.f));
}

_bool CMonsterState_Base::Update_MonsterLook(_float fTimeDelta)
{
	_vector vTargetPos = Calculate_MonsterDir(m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POS));

	return m_pMonster->Get_Transform()->Turn_ToTarget(AXIS_Y, fTimeDelta, vTargetPos);
}

_float CMonsterState_Base::Update_MonsterTurnSpeed(_float fSpeed)
{
	_vector vLook = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);
	_vector vTargetPos = Calculate_MonsterDir(m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POS));

	_float fDot = acosf(XMVectorGetX(XMVector4Dot(vLook, vTargetPos)));

	_float fDegree = XMConvertToDegrees(fDot);
	// 진입했을 때 각도에 따른 속도 설정
	m_pMonster->Get_Transform()->Set_RotationSpeed(fDot * fSpeed);
	
	return fDegree;
}

void CMonsterState_Base::Setting_PlayerInfo()
{
	m_pPlayer = m_pGameInstance->Find_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player"));
}

HRESULT CMonsterState_Base::Check_Dead(_float fTimeDelta)
{
	_bool bDead = m_pMonster->Is_Dead();

	if (bDead)
	{
		m_pMonster->Change_CurrentState(CMonster::STATE_DEAD);
		return E_ABORT;
	}
	
	return S_OK;
}

HRESULT CMonsterState_Base::Check_Hit(_float fTimeDelta)
{
	_bool bHit = m_pMonster->Is_Hit();

	if (bHit && (m_iPreState != CMonster::STATE_ATTACK))
	{
		_vector vPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS);
		_vector vLook = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);
		_vector vPlayerPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POS);

		_vector vTargetPos = XMVector4Normalize(Calculate_MonsterDir(vPlayerPos));

		_float fDegree = XMConvertToDegrees(acosf(XMVectorGetX(XMVector4Dot(vLook, vTargetPos))));

		if (fDegree < 40.f)
		{
			if (m_pGameInstance->Random_Persent(20))
			{
				m_pMonster->Change_CurrentState(CMonster::STATE_HIT);
				return E_ABORT;
			}

		}
		else if(fDegree >= 40.f && !bHit)
		{
			m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
			return E_ABORT;
		}
	}

	return S_OK;
}


void CMonsterState_Base::Free()
{
	__super::Free();
}