#include "Attack_Base.h"
#include "GameInstance.h"
#include "Monster.h"
#include "PartObject.h"

CAttack_Base::CAttack_Base(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base { pOwner,pAnimOwner }
{
}

void CAttack_Base::Update_Animation(_float fTimeDelta)
{
    __super::Update_Animation(fTimeDelta);
}

void CAttack_Base::Regist_PartCollUpdate()
{
    if (m_pMonster->Get_PartObject(CMonster::PART_LEFT) == nullptr)
        m_pGameInstance->Regist_Update(m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());

    else if (m_pMonster->Get_PartObject(CMonster::PART_RIGHT) == nullptr)
        m_pGameInstance->Regist_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder());
    else
        m_pGameInstance->Regist_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder(),
            m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());
}

void CAttack_Base::Secede_PartCollUpdate()
{
    if (m_pMonster->Get_PartObject(CMonster::PART_LEFT) == nullptr)
        m_pGameInstance->Secede_Update(m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());

    else if (m_pMonster->Get_PartObject(CMonster::PART_RIGHT) == nullptr)
        m_pGameInstance->Secede_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder());
    else
        m_pGameInstance->Secede_Update(m_pMonster->Get_PartObject(CMonster::PART_LEFT)->Get_ColliderCom()->Get_Bounder(),
            m_pMonster->Get_PartObject(CMonster::PART_RIGHT)->Get_ColliderCom()->Get_Bounder());

}

void CAttack_Base::Free()
{
    __super::Free();
}

