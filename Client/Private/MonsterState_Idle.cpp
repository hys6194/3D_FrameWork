#include "MonsterState_Idle.h"

#include "Player.h"
#include "Monster.h"
#include "Body_Ghoul.h"
#include "Ghoul.h"

CMonsterState_Idle::CMonsterState_Idle(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base{ pOwner, pAnimOwner }
{
}

HRESULT CMonsterState_Idle::Enter_State()
{ 
    Set_CurAnimation();

    Setting_PlayerInfo();

    return S_OK;
}

void CMonsterState_Idle::PriorityUpdate_State(_float fTimeDelta)
{
    if (FAILED(Check_Dead(fTimeDelta)))
        return;

    if (FAILED(Check_Hit(fTimeDelta)))
        return;

    _vector vPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS);
    _vector vLook = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);
    _vector vPlayerPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POS);

    _vector vTargetPos = XMVector4Normalize(Calculate_MonsterDir(vPlayerPos));

    _float fDegree = XMConvertToDegrees(acosf(XMVectorGetX(XMVector4Dot(vLook, vTargetPos))));

    if (m_pMonster->Is_Hit())
    {
        if (fDegree > 60.f)
            m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
        else
            m_pMonster->Change_CurrentState(CMonster::STATE_TRACE);

        return;
    }

    _float fDistance = XMVectorGetX(XMVector4Length(XMVectorSubtract(vPos, vPlayerPos)));

    if(fDistance < m_fDistance)
    {
        if (fDegree > 60.f)
            m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
        else
            m_pMonster->Change_CurrentState(CMonster::STATE_TRACE);
    }

}

void CMonsterState_Idle::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMonsterState_Idle::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMonsterState_Idle::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void CMonsterState_Idle::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_iPreState = CMonster::STATE_IDLE;
}

void CMonsterState_Idle::Update_Animation(_float fTimeDelta)
{
    __super::Update_Animation(fTimeDelta);
}

void CMonsterState_Idle::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex, true);
}

CMonsterState_Idle* CMonsterState_Idle::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex)
{
    CMonsterState_Idle* pInstance = new CMonsterState_Idle(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = AnimIndex;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance; 
}

void CMonsterState_Idle::Free()
{
    __super::Free();
}
