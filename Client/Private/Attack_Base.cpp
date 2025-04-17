#include "Attack_Base.h"
#include "GameInstance.h"
#include "Player.h"
#include "Monster.h"
#include "PartObject.h"

#include "Attack.h"
#include "Ghoul.h"
#include "Moloch.h"


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

void CAttack_Base::Check_Patterns()
{
    // 보스가 공격 애니메이션이 끝났을 때 플레이어와 내적하여 각도 계산 
    if (m_pMonster->Is_Boss())
    {
        //static_cast<CPlayer*>(m_pPlayer)->

    }

    else
    {

    }
}

void CAttack_Base::Free()
{
    __super::Free();
}

