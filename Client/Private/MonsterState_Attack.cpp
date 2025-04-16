#include "MonsterState_Attack.h"

#include "Player.h"
#include "Ghoul.h"
#include "Moloch.h"
#include "Fist_Left.h"
#include "Fist_Right.h"

#include "Body_Ghoul.h"
#include "GameInstance.h"

#include "FSM.h"
#include "Attack.h"
#include "Attack_Base.h"

CMonsterState_Attack::CMonsterState_Attack(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base{ pOwner, pAnimOwner }
{
}

HRESULT CMonsterState_Attack::Enter_State()
{ 
    m_bAnimEnd = false;
    // 여기에서 AnimIndex설정해줘야 함
    m_pAttackCom = static_cast<CAttack*>(m_pMonster->Get_Component(COM_ATTACK));

    if (nullptr == m_pAttackCom)
    {
        m_pMonster->Change_CurrentState(CMonster::STATE_IDLE);
        static_cast<CFSM*>(m_pMonster->Get_Component(COM_FSM))->Change_State(CMonster::STATE_IDLE);
        return S_OK;
    }

    m_pAttackPattern = m_pAttackCom->Find_Attackable();

    if (nullptr == m_pAttackPattern)
    {
        m_pMonster->Change_CurrentState(CMonster::STATE_IDLE);
        static_cast<CFSM*>(m_pMonster->Get_Component(COM_FSM))->Change_State(CMonster::STATE_IDLE);
        return S_OK;
    }

    m_pAttackPattern->Enter_State();

    return S_OK;
}

void CMonsterState_Attack::PriorityUpdate_State(_float fTimeDelta)
{
    m_pAttackPattern->PriorityUpdate_State(fTimeDelta);
}


void CMonsterState_Attack::Update_State(_float fTimeDelta)
{
    m_pAttackPattern->Update_State(fTimeDelta);
}

void CMonsterState_Attack::LateUpdate_State(_float fTimeDelta)
{
    m_pAttackPattern->LateUpdate_State(fTimeDelta);
}

HRESULT CMonsterState_Attack::Exit_State()
{
    if (nullptr == m_pAttackPattern)
        return E_ABORT;

    m_pAttackPattern->Exit_State();

    return S_OK;
}

void CMonsterState_Attack::Set_PreAnimation()
{
    m_pAttackPattern->Set_PreAnimation();
}

void CMonsterState_Attack::Update_Animation(_float fTimeDelta)
{
    m_pAttackPattern->Update_Animation(fTimeDelta);
}

void CMonsterState_Attack::Set_CurAnimation()
{
    m_pAttackPattern->Set_CurAnimation();
}

CMonsterState_Attack* CMonsterState_Attack::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex)
{
    CMonsterState_Attack* pInstance = new CMonsterState_Attack(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = AnimIndex;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance; 
}

void CMonsterState_Attack::Free()
{
    __super::Free();
}
