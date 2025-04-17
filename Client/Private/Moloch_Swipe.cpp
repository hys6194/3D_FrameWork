#include "Moloch_Swipe.h"
#include "GameInstance.h"
#include "Player.h"

#include "Moloch.h"
#include "Body_Moloch.h"
//#include "Ghoul.h"
//#include "Body_Ghoul.h"

CMoloch_Swipe::CMoloch_Swipe(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base { pOwner, pAnimOwner }
{
}

HRESULT CMoloch_Swipe::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_01;

    m_bAttack = true;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMoloch_Swipe::PriorityUpdate_State(_float fTimeDelta)
{
    if(Check_Dead(fTimeDelta))                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  if (FAILED(Check_Dead(fTimeDelta)))
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

void CMoloch_Swipe::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMoloch_Swipe::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMoloch_Swipe::Exit_State()
{
    Set_PreAnimation();
    Secede_PartCollUpdate();

    m_fElapseTime = 0.f;

    m_bAttack = false;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMoloch_Swipe::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
}

void CMoloch_Swipe::Update_Animation(_float fTimeDelta)
{
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)), 0.5f);
}

void CMoloch_Swipe::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CMoloch_Swipe::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack /*|| m_fCoolTime이 원래 쿨타임과 같아지거나 커졌다면 */)
        return;
    else
    {
        m_fElapseTime += fTimeDelta;
    }
}

_bool CMoloch_Swipe::Check_Attackable()
{
    // 쿨 다 돌면
    if (m_fElapseTime >= m_fCoolTime)
        return true;

    // 공격 조건이라면
    if (Check_Colls())
        return true;

    return false;
}

_bool CMoloch_Swipe::Check_Colls()
{

    Setting_PlayerInfo();


    if (m_iAnimIndex == CMoloch::MOLOCH_ATK_SWIPE_03)
    {
        if(m_bAnimEnd)
        {
            m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
            m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_01;

            Secede_PartCollUpdate();

            m_bRegisted = false;
            m_bSeceded = false;
            m_fTotalTime = 0.f;

            return false;
        }
    }

    else if (m_pModelCom->Get_CurAnimationTrackPosition() >= 33)
    {
        _float fDegree = XMConvertToDegrees(Get_MonsterLookDot());

        if (30 > fDegree)
        {
            m_iAnimIndex == CMoloch::MOLOCH_ATK_SWIPE_01 ?
                m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_02 : m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_03;

            m_pModelCom->Set_AnimationIndex(m_iAnimIndex);

            Secede_PartCollUpdate();

            m_bRegisted = false;
            m_bSeceded = false;
            m_fTotalTime = 0.f;

            int a = 10;

            return true;
        }

        else if(30 <= fDegree && m_bAnimEnd)
        {
            m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
            m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_01;

            Secede_PartCollUpdate();

            m_bRegisted = false;
            m_bSeceded = false;
            m_fTotalTime = 0.f;

            return false;
        }
        
    }

    return false;
}

CMoloch_Swipe* CMoloch_Swipe::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime)
{
    CMoloch_Swipe* pInstance = new CMoloch_Swipe(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;
    pInstance->m_fCoolTime = fCoolTime;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CMoloch_Swipe::Free()
{
    __super::Free();
}
