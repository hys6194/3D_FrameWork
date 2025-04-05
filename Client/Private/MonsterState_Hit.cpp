#include "MonsterState_Hit.h"

#include "Player.h"
#include "Monster.h"
#include "Body_Ghoul.h"
#include "Ghoul.h"

CMonsterState_Hit::CMonsterState_Hit(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base{ pOwner, pAnimOwner }
{
}

HRESULT CMonsterState_Hit::Enter_State()
{ 
    Setting_PlayerInfo();
    Set_CurAnimation();

    return S_OK;
}

void CMonsterState_Hit::PriorityUpdate_State(_float fTimeDelta)
{
    // 이 클래스에서는 몬스터가 치명타를 맞게 되면 애니메이션을 재생하고
    // 일정 50퍼센트 확률로 AVOID 상태로 이동
    if (FAILED(Check_Dead(fTimeDelta)))
        return;

    if (m_bAnimEnd)
    {
        if (m_pGameInstance->Random_Persent(50))
            m_pMonster->Change_CurrentState(CMonster::STATE_AVOID);
        else
            m_pMonster->Change_CurrentState(CMonster::STATE_IDLE);
    }


}


void CMonsterState_Hit::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMonsterState_Hit::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMonsterState_Hit::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void CMonsterState_Hit::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_iPreState = CMonster::STATE_HIT;
    m_bAnimEnd = false;

}

void CMonsterState_Hit::Update_Animation(_float fTimeDelta)
{
    __super::Update_Animation(fTimeDelta);
}

void CMonsterState_Hit::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

CMonsterState_Hit* CMonsterState_Hit::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex)
{
    CMonsterState_Hit* pInstance = new CMonsterState_Hit(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = AnimIndex;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance; 
}

void CMonsterState_Hit::Free()
{
    __super::Free();
}
