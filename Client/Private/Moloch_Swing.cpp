#include "Moloch_Swing.h"
#include "GameInstance.h"
#include "Player.h"

#include "Moloch.h"
#include "Body_Moloch.h"
//#include "Ghoul.h"
//#include "Body_Ghoul.h"

CMoloch_Swing::CMoloch_Swing(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base { pOwner, pAnimOwner }
{
}

HRESULT CMoloch_Swing::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    m_bAttack = true;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMoloch_Swing::PriorityUpdate_State(_float fTimeDelta)
{
    if(Check_Dead(fTimeDelta))                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  if (FAILED(Check_Dead(fTimeDelta)))
        return;

    // 충돌체의 업데이트 시간
    switch (m_iAnimIndex)
    {
        case CMoloch::MOLOCH_ATK_FULL_SWING_01:
            if ((22 <= m_pModelCom->Get_CurAnimationTrackPosition() &&
                m_pModelCom->Get_CurAnimationTrackPosition() <= 41) && 
                !m_bRegisted)
            {
                Regist_PartCollUpdate();
                m_bRegisted = true;
            }

            else if (!(22 <= m_pModelCom->Get_CurAnimationTrackPosition() &&
                m_pModelCom->Get_CurAnimationTrackPosition() <= 41) &&
                !m_bSeceded)
            {
                Secede_PartCollUpdate();
                m_bSeceded = true;
            }

            break;

        case CMoloch::MOLOCH_ATK_FULL_SWING_02:
            if ((20 <= m_pModelCom->Get_CurAnimationTrackPosition() &&
                m_pModelCom->Get_CurAnimationTrackPosition() <= 45) &&
                !m_bRegisted)
            {
                Regist_PartCollUpdate();
                m_bRegisted = true;
            }

            else if (!(20 <= m_pModelCom->Get_CurAnimationTrackPosition() &&
                m_pModelCom->Get_CurAnimationTrackPosition() <= 45) &&
                !m_bSeceded)
            {
                Secede_PartCollUpdate();
                m_bSeceded = true;
            }

            break;

        case CMoloch::MOLOCH_ATK_FULL_SWING_03:
            if ((35 <= m_pModelCom->Get_CurAnimationTrackPosition() &&
                m_pModelCom->Get_CurAnimationTrackPosition() <= 70) &&
                !m_bRegisted)
            {
                Regist_PartCollUpdate();
                m_bRegisted = true;
            }

            else if (!(35 <= m_pModelCom->Get_CurAnimationTrackPosition() &&
                m_pModelCom->Get_CurAnimationTrackPosition() <= 70) &&
                !m_bSeceded)
            {
                Secede_PartCollUpdate();
                m_bSeceded = true;
            }

            break;
    }

    if (m_bAnimEnd)
    {
        m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
    
        Secede_PartCollUpdate();
    
        m_bRegisted = false;
        m_bSeceded = false;
        m_fTotalTime = 0.f;
        
        return;
    }
    
    // 애니메이션 돌아갈 때 히트 판정의 발동과 해제
    if (0.65f < m_fTotalTime && !m_bRegisted)
    {
        Regist_PartCollUpdate();
        m_bRegisted = true;
    }
    
    // 애니메이션의 중간에 이벤트를 발생해서 Update 등록 및 해제를 하고 싶은데 안되나
    if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() / 1.5f &&
        !m_bSeceded)
    {
        Secede_PartCollUpdate();
        m_bSeceded = true;
    }

}

void CMoloch_Swing::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMoloch_Swing::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMoloch_Swing::Exit_State()
{
    Set_PreAnimation();
    Secede_PartCollUpdate();

    m_fElapseTime = 0.f;

    m_bAttack = false;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMoloch_Swing::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
}

void CMoloch_Swing::Update_Animation(_float fTimeDelta)
{
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)), 0.5f);
}

void CMoloch_Swing::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    _uint iNum = m_pGameInstance->Draw_RandomNum(3);

    if (1 == iNum)
        m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_SWING_01;

    else if (2 == iNum)
        m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_SWING_02;

    else if (3 == iNum)
        m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_SWING_03;

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CMoloch_Swing::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack /*|| m_fCoolTime이 원래 쿨타임과 같아지거나 커졌다면 */)
        return;
    else
    {
        m_fElapseTime += fTimeDelta;
    }
}

_bool CMoloch_Swing::Check_Attackable()
{
    // 쿨 다 돌면
    if (m_fElapseTime >= m_fCoolTime && Check_Colls())
        return true;

    // 공격 조건이라면
    if (Check_Colls())
        return true;

    return false;
}

_bool CMoloch_Swing::Check_Colls()
{
    Setting_PlayerInfo();

    _bool bColl1 = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
    _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

    if (bColl1 && bColl2)
        return true;

    return false;
}

CMoloch_Swing* CMoloch_Swing::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime)
{
    CMoloch_Swing* pInstance = new CMoloch_Swing(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;
    pInstance->m_fCoolTime = fCoolTime;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CMoloch_Swing::Free()
{
    __super::Free();
}
