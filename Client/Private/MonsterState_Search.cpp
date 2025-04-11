#include "MonsterState_Search.h"

#include "Player.h"
#include "Monster.h"
#include "Body_Ghoul.h"
#include "Ghoul.h"
#include "GameInstance.h"

CMonsterState_Search::CMonsterState_Search(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base{ pOwner, pAnimOwner }
{
}

HRESULT CMonsterState_Search::Enter_State()
{ 
    Setting_PlayerInfo();

    Set_CurAnimation();
    // 진입했을 때 플레이어와 몬스터의 각도로 회전 속도 설정

    m_iPreState = m_pMonster->Get_PreState();

    return S_OK;
}

void CMonsterState_Search::PriorityUpdate_State(_float fTimeDelta)
{
    // 이게 맞나? 차라리 Base에 그냥 함수로 만들어서 호출하는게 훨 나아보이기도 하고
    if (FAILED(Check_Dead(fTimeDelta)))
        return;

    // 만약 이전 State가 Avoid였다면
    if(m_iPreState == CMonster::STATE_AVOID && !m_bCheck)
    {
        m_bCheck = true;

        _float fDegree = Update_MonsterTurnSpeed();
    
        // 해당 각도가 50안에 들어와있었다면
        if (50.f > fDegree)
        {
            m_pMonster->Change_CurrentState(CMonster::STATE_TRACE);
            return;
        }
    
    }

    _vector vTargetPos = Calculate_MonsterDir(m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POS));

    _bool bTurn = m_pMonster->Get_Transform()->Turn_ToTarget(AXIS_Y, fTimeDelta, vTargetPos);

    if(bTurn || m_bAnimEnd)
        m_pMonster->Change_CurrentState(CMonster::STATE_TRACE);

}

void CMonsterState_Search::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMonsterState_Search::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMonsterState_Search::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void CMonsterState_Search::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_SEARCH);
    m_bAnimEnd = false;
    m_bCheck = false;
}

void CMonsterState_Search::Update_Animation(_float fTimeDelta)
{
    // 회전량 없애기 위해서
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);
}

void CMonsterState_Search::Set_CurAnimation()
{
    _float fDegree = Update_MonsterTurnSpeed();

    if(0 <= fDegree)
        m_pModelCom->Set_AnimationIndex(m_iAnimIndex + 1);
    else
        m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

CMonsterState_Search* CMonsterState_Search::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex)
{
    CMonsterState_Search* pInstance = new CMonsterState_Search(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = AnimIndex;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance; 
}

void CMonsterState_Search::Free()
{
    __super::Free();
}
