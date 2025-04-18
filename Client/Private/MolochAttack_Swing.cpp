#include "MolochAttack_Swing.h"
#include "GameInstance.h"
#include "Player.h"

#include "Moloch.h"
#include "Crystal.h"
#include "Body_Moloch.h"
//#include "Ghoul.h"
//#include "Body_Ghoul.h"

CMolochAttack_Swing::CMolochAttack_Swing(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base { pOwner, pAnimOwner }
{
}

HRESULT CMolochAttack_Swing::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    m_bAttack = true;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMolochAttack_Swing::PriorityUpdate_State(_float fTimeDelta)
{
    if(Check_Dead(fTimeDelta))                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  if (FAILED(Check_Dead(fTimeDelta)))
        return;

    // 충돌체의 업데이트 시간
    switch (m_iAnimIndex)
    {
        case CMoloch::MOLOCH_ATK_FULL_SWING_01:
            Regist_CollUpdate(22, 41);
            Secede_CollUpdate(22, 41);
            break;

        case CMoloch::MOLOCH_ATK_FULL_SWING_02:
            Regist_CollUpdate(20, 45);
            Secede_CollUpdate(20, 45);
            break;

        case CMoloch::MOLOCH_ATK_FULL_SWING_03:      
            Regist_CollUpdate(35, 70);
            Secede_CollUpdate(35, 70);

            CCrystal::CRYSTAL_DESC Desc{};
            Desc.vLook = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);
            Desc.strModelTag = PRO_MODEL_MOLOCH_CRYSTAL_A;
            Desc.pOwner = dynamic_cast<CMoloch*>(m_pMonster);

            _float fCount = 9.f;
            _float fOffset = 4.5f;

            // 원뿔 형태로 만들어야 함
            if (42.f <= m_pModelCom->Get_CurAnimationTrackPosition() && !m_bSpawn)
            {
                Desc.strModelTag = PRO_MODEL_MOLOCH_CRYSTAL_B;
                m_bSpawn = true;

                for (size_t i = 0; i < fCount; i++)
                {
                    for (size_t j = 0; j < i; j++)
                    {
                        _vector vTest = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS);
                        vTest += Desc.vLook * (fOffset * (i + 1));

                        Desc.vCrystalPos = vTest;

                        FAILED_CHECK_RETURN(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_CRYSTAL,
                            LEVEL_GAMEPLAY, Desc.strModelTag, &Desc), );
                    }
                }
            }
            break;
    }

    if (m_bAnimEnd)
    {    
        Secede_PartCollUpdate();
    
        m_bRegisted = false;
        m_bSeceded = false;
        m_fTotalTime = 0.f;
        
        return;
    }
}

void CMolochAttack_Swing::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMolochAttack_Swing::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMolochAttack_Swing::Exit_State()
{
    Set_PreAnimation();
    Secede_PartCollUpdate();

    m_fElapseTime = 0.f;

    m_bAttack = false;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMolochAttack_Swing::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
}

void CMolochAttack_Swing::Update_Animation(_float fTimeDelta)
{
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)), 0.5f);
}

void CMolochAttack_Swing::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    _uint iNum = m_pGameInstance->Draw_RandomNum(3);

    if (1 == iNum)
        m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_SWING_01;

    else if (2 == iNum)
        m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_SWING_02;

    else if (3 == iNum)
        m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_SWING_03;

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CMolochAttack_Swing::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack /*|| m_fCoolTime이 원래 쿨타임과 같아지거나 커졌다면 */)
        return;
    else
    {
        m_fElapseTime += fTimeDelta;
    }
}

_bool CMolochAttack_Swing::Check_Attackable()
{
    // 쿨 다 돌면
    if (m_fElapseTime >= m_fCoolTime && Check_Colls())
        return true;

    //// 공격 조건이라면
    //if (Check_Colls())
    //    return true;

    return false;
}

_bool CMolochAttack_Swing::Check_Colls()
{
    Setting_PlayerInfo();

    _bool bColl1 = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
    _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

    if (bColl1 && bColl2)
        return true;

    return false;
}

CMolochAttack_Swing* CMolochAttack_Swing::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime)
{
    CMolochAttack_Swing* pInstance = new CMolochAttack_Swing(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;
    pInstance->m_fCoolTime = fCoolTime;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CMolochAttack_Swing::Free()
{
    __super::Free();
}
