#include "GhoulAttack_Flurry.h"
#include "GameInstance.h"
#include "Player.h"

#include "Ghoul.h"
#include "Body_Ghoul.h"

CGhoulAttack_Flurry::CGhoulAttack_Flurry(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base{ pOwner, pAnimOwner }
{
}

HRESULT CGhoulAttack_Flurry::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    m_iAnimIndex = CGhoul::GHOUL_ATK_FLURRY;

    m_bAttack = true;

    return S_OK;
}


void CGhoulAttack_Flurry::PriorityUpdate_State(_float fTimeDelta)
{
    if (FAILED(Check_Dead(fTimeDelta)))
        return;

    Check_Condition();

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

    // 애니메이션의 중간에 이벤트를 발생해서 Update 등록 및 해제를 하고 싶은데 안되나
    if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() / 1.5f &&
        !m_bSeceded)
    {
        Secede_PartCollUpdate();
        m_bSeceded = true;
    }
}

void CGhoulAttack_Flurry::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));
}

void CGhoulAttack_Flurry::LateUpdate_State(_float fTimeDelta)
{    
  

}

HRESULT CGhoulAttack_Flurry::Exit_State()
{
    Set_PreAnimation();
    Secede_PartCollUpdate();

    m_fElapseTime = 0.f;
    m_bAttack = false;

    return S_OK;
}

void CGhoulAttack_Flurry::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
    m_bAnimEnd = false;
}

void CGhoulAttack_Flurry::Update_Animation(_float fTimeDelta)
{
      //if (0 != m_pModelCom->Get_PreAnimIndex()
      //    && m_pModelCom->Get_Interpolate())
      //    m_pModelCom->Interpolate_Animation(0.2f);
      //else
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));
}

void CGhoulAttack_Flurry::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CGhoulAttack_Flurry::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack /*|| m_fCoolTime이 원래 쿨타임과 같아지거나 커졌다면 */)
        return;
    else
    {
        m_fElapseTime += fTimeDelta;
    }
}

_bool CGhoulAttack_Flurry::Check_Attackable()
{
    if (m_fElapseTime < m_fCoolTime)
        return false;

    if (!Check_Condition())
        return false;

    return true;
}

_bool CGhoulAttack_Flurry::Check_Condition()
{
    Setting_PlayerInfo();


    _bool bColl = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
    _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

    if (bColl && bColl2)
        return true;

    return false;
}

CGhoulAttack_Flurry* CGhoulAttack_Flurry::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime)
{
    CGhoulAttack_Flurry* pInstance = new CGhoulAttack_Flurry(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;
    pInstance->m_fCoolTime = fCoolTime;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CGhoulAttack_Flurry::Free()
{
    __super::Free();
}
