#include "MonsterState_Base.h"

#include "Monster.h"
#include "Body_Ghoul.h"

CMonsterState_Base::CMonsterState_Base(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
	: CState{ pDevice, pContext, pOwner, pAnimOwner, m_pGameInstance }
{
	// 이래도 되려나	
	m_pMonster = dynamic_cast<CMonster*>(m_pOwner);
	m_pBody    = dynamic_cast<CBody_Ghoul*>(m_pAnimOwner);
}

HRESULT CMonsterState_Base::Enter_State()
{
	return S_OK;
}

void CMonsterState_Base::PriorityUpdate_State(_float fTimeDelta)
{
	// 몬스터가 죽었는지 안 죽었는지 알아야 함
	_bool bDead = m_pMonster->Is_Dead();

	if (bDead)
	{
		m_pMonster->Change_CurrentState(CMonster::STATE_DEAD);
		return;
	}

	// 피격 판정 어케해야하지 진짜
}

void CMonsterState_Base::Update_Animation(_float fTimeDelta)
{
	// 가상함수로 기본적인 애니메이션은 이걸로 돌리고 루트애님이나 그런거 들어간건 override해서 쓰기
	if (0 != m_pModelCom->Get_PreAnimIndex()
		&& m_pModelCom->Get_Interpolate())
		m_pModelCom->Interpolate_Animation(0.2f);
	else
		m_pModelCom->Play_Animation(fTimeDelta);
}


void CMonsterState_Base::Free()
{
	__super::Free();
}