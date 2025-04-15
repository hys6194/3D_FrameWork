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


    Setting_PlayerInfo();
    Set_CurAnimation();


    return S_OK;
}

void CMonsterState_Attack::PriorityUpdate_State(_float fTimeDelta)
{
    
    m_pAttackPattern->PriorityUpdate_State(fTimeDelta);

    //if (FAILED(Check_Dead(fTimeDelta)))
    //    return;
    //if (m_pMonster->Is_Boss())
    //{
    //    // 이거 자체가 문제일 수도 있음
    //    // 아니다 그냥 
    //
    //    if(m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() / 2)
    //    {
    //        if (m_iAnimIndex == CMoloch::MOLOCH_ATK_SWIPE_03)
    //        {
    //            m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
    //            m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_01;
    //
    //            Secede_PartCollUpdate();
    //
    //            m_bRegisted = false;
    //            m_bSeceded = false;
    //            m_fTotalTime = 0.f;
    //
    //            return;
    //        }
    //
    //        else
    //        {
    //            _float fDegree = XMConvertToDegrees(Get_MonsterLookDot());
    //            if (30 > fDegree)
    //            {
    //                m_iAnimIndex == CMoloch::MOLOCH_ATK_SWIPE_01 ?
    //                    m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_02 : m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_03;
    //
    //
    //                int a = 10;
    //            }
    //
    //            else
    //            {
    //                m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
    //                m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_01;
    //
    //                Secede_PartCollUpdate();
    //
    //                m_bRegisted = false;
    //                m_bSeceded = false;
    //                m_fTotalTime = 0.f;
    //
    //                return;
    //            }
    //        }
    //    }
    //
    //    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
    //}
    //
    //if (m_bAnimEnd)
    //{
    //
    //    m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
    //
    //
    //    Secede_PartCollUpdate();
    //
    //    m_bRegisted = false;
    //    m_bSeceded = false;
    //    m_fTotalTime = 0.f;
    //    
    //    return;
    //}
    //
    //m_fTotalTime += m_pGameInstance->Get_TimeDelta(TIME60);
    //
    //if (0.65f < m_fTotalTime && !m_bRegisted)
    //{
    //    Regist_PartCollUpdate();
    //    m_bRegisted = true;
    //}
    //
    //// 애니메이션의 중간에 이벤트를 발생해서 Update 등록 및 해제를 하고 싶은데 안되나
    //if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() / 1.5f &&
    //    !m_bSeceded)
    //{
    //    Secede_PartCollUpdate();
    //    m_bSeceded = true;
    //}


}


void CMonsterState_Attack::Update_State(_float fTimeDelta)
{
    m_pAttackPattern->Update_State(fTimeDelta);

    //Update_Animation(fTimeDelta);
    //
    //m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));
}

void CMonsterState_Attack::LateUpdate_State(_float fTimeDelta)
{
    m_pAttackPattern->LateUpdate_State(fTimeDelta);

}

HRESULT CMonsterState_Attack::Exit_State()
{
    //Set_PreAnimation();
    //Secede_PartCollUpdate();
    m_pAttackPattern->Exit_State();

    return S_OK;
}

void CMonsterState_Attack::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
    m_bAnimEnd = false;
}

void CMonsterState_Attack::Update_Animation(_float fTimeDelta)
{
    //if (0 != m_pModelCom->Get_PreAnimIndex()
    //    && m_pModelCom->Get_Interpolate())
    //    m_pModelCom->Interpolate_Animation(0.2f);
    //else
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    if (m_bAnimEnd)
        int a = 10;

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));
}

void CMonsterState_Attack::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CMonsterState_Attack::Regist_PartCollUpdate()
{
    if (m_pMonster->Get_PartObject(CMonster::PART_LEFT) == nullptr)
        m_pGameInstance->Regist_Update(m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());

    else if (m_pMonster->Get_PartObject(CMonster::PART_RIGHT) == nullptr)
        m_pGameInstance->Regist_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder());
    else
        m_pGameInstance->Regist_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder(),
            m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());
}

void CMonsterState_Attack::Secede_PartCollUpdate()
{
    if (m_pMonster->Get_PartObject(CMonster::PART_LEFT) == nullptr)
        m_pGameInstance->Secede_Update(m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());

    else if (m_pMonster->Get_PartObject(CMonster::PART_RIGHT) == nullptr)
        m_pGameInstance->Secede_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder());
    else
        m_pGameInstance->Secede_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder(),
            m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());

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
