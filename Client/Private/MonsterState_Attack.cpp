#include "MonsterState_Attack.h"

#include "Player.h"
#include "Monster.h"
#include "Body_Ghoul.h"
#include "Ghoul.h"
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

    // 애니메이션 중간에 m_pGameInstance->Regist_Update로 충돌원의 Update에 등록
    if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() / 2.5f)
    {
        // 해당 객체의 Part 객체를 가져와서 해당 객체의 Part오브젝트의 Collision을 가져와야 한다
        // 그러면 어떻게 해야 가져올 수 있을까?
        // 그러면 Get_Part를 하고 함수 구조를 
        // 
        // CollCom의 형태가 다 각자 각기가 될 수 있기 때문에 이렇게 수정
        // 근데 Com에 접근을 하고나서 이후에 어떤식으로 해야할까 Component가 
        //m_pGameInstance->Regist_Update()
        m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_Component(COM_COLL);
        m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_Component(COM_COLL);
    }

    // 플레이어 몸 충돌체와 자신의 몸 충돌체 체크를 통해서
    // 곂쳐있다면 뒤로 빠지거나 Avoid로
    // 안 곂쳐있다면 바로 Search


    // 애니메이션이 끝나면 상태를 변환을 하는데 왜 같은 상태를 공유하게 되지?
    // 만약 공유를 해왔더라면 처음부터 모든 몬스터들이 같은 애니메이션을 진행할 텐데


    if (m_bAnimEnd)
        m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
}


void CMonsterState_Attack::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));

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
