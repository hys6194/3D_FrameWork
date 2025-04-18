#include "GhoulAttack_DoubleSwipe.h"
#include "GameInstance.h"
#include "Player.h"

#include "Ghoul.h"
#include "Body_Ghoul.h"

CGhoulAttack_DoubleSwipe::CGhoulAttack_DoubleSwipe(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base{ pOwner, pAnimOwner }
{
}

HRESULT CGhoulAttack_DoubleSwipe::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    // 해당 State가 공격에 들어갔다면 쿨타임 체크를 안돌게 한다
    m_bAttack = true;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}


void CGhoulAttack_DoubleSwipe::PriorityUpdate_State(_float fTimeDelta)
{
    if (FAILED(Check_Dead(fTimeDelta)))
        return;

    Check_Colls();

    if (m_bAnimEnd)
    {
        m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);

        Secede_PartCollUpdate();

        m_bRegisted = false;
        m_bSeceded = false;
        m_fTotalTime = 0.f;


        return;
    }

    m_fTotalTime += m_pGameInstance->Get_TimeDelta(TIME60);

    if (0.65f < m_fTotalTime && !m_bRegisted)
    {
        Regist_PartCollUpdate();
        m_bRegisted = true;
    }

    if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() / 1.5f &&
        !m_bSeceded)
    {
        Secede_PartCollUpdate();
        m_bSeceded = true;
    }
}

void CGhoulAttack_DoubleSwipe::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));
}

void CGhoulAttack_DoubleSwipe::LateUpdate_State(_float fTimeDelta)
{    
  

}

HRESULT CGhoulAttack_DoubleSwipe::Exit_State()
{
    Set_PreAnimation();
    Secede_PartCollUpdate();

    m_fElapseTime = 0.f;
    m_bAttack = false;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CGhoulAttack_DoubleSwipe::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
    m_bAnimEnd = false;
}

void CGhoulAttack_DoubleSwipe::Update_Animation(_float fTimeDelta)
{
      //if (0 != m_pModelCom->Get_PreAnimIndex()
      //    && m_pModelCom->Get_Interpolate())
      //    m_pModelCom->Interpolate_Animation(0.2f);
      //else
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));
}

void CGhoulAttack_DoubleSwipe::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CGhoulAttack_DoubleSwipe::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack)
        return;

    else
    {
        m_fElapseTime += fTimeDelta;
    }
}

_bool CGhoulAttack_DoubleSwipe::Check_Attackable()
{
    if (m_fElapseTime < m_fCoolTime)
        return false;

    if (!Check_Colls())
        return false;

    return true;
}

_bool CGhoulAttack_DoubleSwipe::Check_Colls()
{
    Setting_PlayerInfo();

    _bool bColl = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
    _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

    if (bColl && bColl2)
        return true;

    return false;
}

CGhoulAttack_DoubleSwipe* CGhoulAttack_DoubleSwipe::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime)
{
    CGhoulAttack_DoubleSwipe* pInstance = new CGhoulAttack_DoubleSwipe(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;
    pInstance->m_fCoolTime = fCoolTime;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CGhoulAttack_DoubleSwipe::Free()
{
    __super::Free();
}
