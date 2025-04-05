#include "MonsterState_Trace.h"

#include "Player.h"
#include "Monster.h"
#include "Body_Ghoul.h"
#include "Ghoul.h"

CMonsterState_Trace::CMonsterState_Trace(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base{ pOwner, pAnimOwner }
{
}

HRESULT CMonsterState_Trace::Enter_State()
{ 
    Set_CurAnimation();

    m_pPlayer = m_pGameInstance->Find_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player"));

    return S_OK;
}

void CMonsterState_Trace::PriorityUpdate_State(_float fTimeDelta)
{
    // 이게 맞나? 차라리 Base에 그냥 함수로 만들어서 호출하는게 훨 나아보이기도 하고
    __super::PriorityUpdate_State(fTimeDelta);

    _vector vPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS);
    _vector vPlayerPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POS);

    _float fDistanace = XMVectorGetX(XMVector4Length(XMVectorSubtract(vPos, vPlayerPos)));


    //if (m_bAnimEnd)
    //{
    //    m_pMonster->Change_CurrentState(CMonster::STATE_IDLE);
    //}


}


void CMonsterState_Trace::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMonsterState_Trace::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMonsterState_Trace::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void CMonsterState_Trace::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
}

void CMonsterState_Trace::Update_Animation(_float fTimeDelta)
{
    __super::Update_Animation(fTimeDelta);
}

void CMonsterState_Trace::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex,true);
}

CMonsterState_Trace* CMonsterState_Trace::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex)
{
    CMonsterState_Trace* pInstance = new CMonsterState_Trace(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = AnimIndex;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance; 
}

void CMonsterState_Trace::Free()
{
    __super::Free();
}
