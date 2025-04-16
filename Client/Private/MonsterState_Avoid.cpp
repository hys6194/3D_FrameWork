#include "MonsterState_Avoid.h"

#include "Player.h"
#include "Monster.h"
#include "Body_Ghoul.h"
#include "Ghoul.h"
#include "GameInstance.h"

CMonsterState_Avoid::CMonsterState_Avoid(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base{ pOwner, pAnimOwner }
{

}

HRESULT CMonsterState_Avoid::Enter_State()
{ 

    Setting_PlayerInfo();

    Set_CurAnimation();
    return S_OK;
}

void CMonsterState_Avoid::PriorityUpdate_State(_float fTimeDelta)
{
    // 이게 맞나? 차라리 Base에 그냥 함수로 만들어서 호출하는게 훨 나아보이기도 하고
    if (FAILED(Check_Dead(fTimeDelta)))
        return;

    _vector vPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS);
    _vector vPlayerPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POS);

    _float fDistanace = XMVectorGetX(XMVector4Length(XMVectorSubtract(vPos, vPlayerPos)));

    if (m_bAnimEnd)
        m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);

}


void CMonsterState_Avoid::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);

     
}

void CMonsterState_Avoid::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMonsterState_Avoid::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void CMonsterState_Avoid::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_AVOID);

    m_bAnimEnd = false;
}

void CMonsterState_Avoid::Update_Animation(_float fTimeDelta)
{
    if (0 != m_pModelCom->Get_PreAnimIndex()
        && m_pModelCom->Get_Interpolate())
        m_pModelCom->Interpolate_Animation(0.2f);
    else
        m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    // 여기에서 옆으로 이동하는 기능 만들어야 함
    m_pMonster->Get_Transform()->Avoid(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));

}

void CMonsterState_Avoid::Set_CurAnimation()
{
    _bool bTemp = m_pGameInstance->Random_Persent(50);

    // 반반 랜덤으로 적용
    if (bTemp)
        m_pModelCom->Set_AnimationIndex(m_iAnimIndex + 1);
    else
        m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

CMonsterState_Avoid* CMonsterState_Avoid::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex)
{
    CMonsterState_Avoid* pInstance = new CMonsterState_Avoid(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = AnimIndex;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance; 
}

void CMonsterState_Avoid::Free()
{
    __super::Free();
}
