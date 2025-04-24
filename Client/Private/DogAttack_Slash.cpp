#include "DogAttack_Slash.h"
#include "GameInstance.h"
#include "Player.h"

#include "Fallen_Dog.h"
#include "Body_FallenDog.h"

CDogAttack_Slash::CDogAttack_Slash(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base{ pOwner, pAnimOwner }
{
}

HRESULT CDogAttack_Slash::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    // 해당 State가 공격에 들어갔다면 쿨타임 체크를 안돌게 한다
    m_bAttack = true;
    m_pMonster->Set_Attack(m_bAttack);

    Set_RandomSound(7, TEXT("Fallendog_atk_slash_"), SOUND_DOG_SLASH, 0.05f, false);
    Set_RandomSound(6, TEXT("Fallendog_atk_slash_vo_"), SOUND_DOG_SLASH_VOICE, 0.05f, false);

    return S_OK;
}


void CDogAttack_Slash::PriorityUpdate_State(_float fTimeDelta)
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

void CDogAttack_Slash::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));
}

void CDogAttack_Slash::LateUpdate_State(_float fTimeDelta)
{

}

HRESULT CDogAttack_Slash::Exit_State()
{
    Set_PreAnimation();
    Secede_PartCollUpdate();

    m_pGameInstance->Stop_Sound(SOUND_DOG_SLASH);
    m_pGameInstance->Stop_Sound(SOUND_DOG_SLASH_VOICE);

    m_fElapseTime = 0.f;
    m_bAttack = false;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CDogAttack_Slash::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
    m_bAnimEnd = false;
}

void CDogAttack_Slash::Update_Animation(_float fTimeDelta)
{
      //if (0 != m_pModelCom->Get_PreAnimIndex()
      //    && m_pModelCom->Get_Interpolate())
      //    m_pModelCom->Interpolate_Animation(0.2f);
      //else
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));
}

void CDogAttack_Slash::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    _uint iNum = m_pGameInstance->Draw_RandomNum(2);

    if (1 == iNum)
        m_iAnimIndex = CFallen_Dog::FALLENDOG_ATK_SLASH_L;
    else
        m_iAnimIndex = CFallen_Dog::FALLENDOG_ATK_SLASH_R;

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CDogAttack_Slash::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack)
        return;

    else
    {
        m_fElapseTime += fTimeDelta;
    }
}

_bool CDogAttack_Slash::Check_Attackable()
{
    if (m_fElapseTime < m_fCoolTime)
        return false;

    if (!Check_Colls())
        return false;

    return true;
}

_bool CDogAttack_Slash::Check_Colls()
{
    Setting_PlayerInfo();

    _bool bColl = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
    _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

    if (!bColl && !bColl2)
        return true;

    return false;
}

CDogAttack_Slash* CDogAttack_Slash::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime)
{
    CDogAttack_Slash* pInstance = new CDogAttack_Slash(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;
    pInstance->m_fCoolTime = fCoolTime;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CDogAttack_Slash::Free()
{
    __super::Free();
}
