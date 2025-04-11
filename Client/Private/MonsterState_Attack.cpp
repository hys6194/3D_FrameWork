#include "MonsterState_Attack.h"

#include "Player.h"
#include "Ghoul.h"
#include "Fist_Left.h"
#include "Fist_Right.h"

#include "Body_Ghoul.h"
#include "GameInstance.h"

CMonsterState_Attack::CMonsterState_Attack(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base{ pOwner, pAnimOwner }
{
}

HRESULT CMonsterState_Attack::Enter_State()
{ 
    Setting_PlayerInfo();
    Set_CurAnimation();



    return S_OK;
}

void CMonsterState_Attack::PriorityUpdate_State(_float fTimeDelta)
{
    // 이게 맞나? 차라리 Base에 그냥 함수로 만들어서 호출하는게 훨 나아보이기도 하고
    if (FAILED(Check_Dead(fTimeDelta)))
        return;

    m_fTotalTime += m_pGameInstance->Get_TimeDelta(TIME60);

    if (0.65f < m_fTotalTime && !m_bRegisted)
    {

        m_pGameInstance->Regist_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder(),
            m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());

        m_bRegisted = true;

    }
    
    if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() / 1.5f &&
        !m_bSeceded)
    {
        m_bSeceded = true;
        m_pGameInstance->Secede_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder(),
            m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());
    }


    if (m_bAnimEnd)
    {
        m_pGameInstance->Secede_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder(),
            m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());
        m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);

        m_bRegisted = false;
        m_fTotalTime = 0.f;
    }
}


void CMonsterState_Attack::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);

    //m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));

}

void CMonsterState_Attack::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMonsterState_Attack::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void CMonsterState_Attack::Set_PreAnimation()
{
    // 모든 키프레임과 bool 타입을 초기화 함으로서 다음에 진입할 때 애니메이션이
    // 제대로 재생이 안될 수 있는 가능성을 제거했는데 왜 다른 객체의 모델까지 공유가 되는거냐고

    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
    m_bAnimEnd = false;
}

void CMonsterState_Attack::Update_Animation(_float fTimeDelta)
{
    if (0 != m_pModelCom->Get_PreAnimIndex()
        && m_pModelCom->Get_Interpolate())
        m_pModelCom->Interpolate_Animation(0.2f);
    else
        m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);
}

void CMonsterState_Attack::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
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
