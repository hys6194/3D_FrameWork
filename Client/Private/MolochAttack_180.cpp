#include "MolochAttack_180.h"
#include "GameInstance.h"
#include "Player.h"

#include "Moloch.h"
#include "Body_Moloch.h"
#include "Crystal.h"

CMolochAttack_180::CMolochAttack_180(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base{ pOwner, pAnimOwner }
{
}

HRESULT CMolochAttack_180::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    m_bAnimEnd = false;

    // 여기에서 몬스터가 회전을 할텐데 회전 속도를 먼저 부여해야 할 듯 하다
    Update_MonsterTurnSpeed();

    m_bAttack = true;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}


void CMolochAttack_180::PriorityUpdate_State(_float fTimeDelta)
{
    if (FAILED(Check_Dead(fTimeDelta)))
        return;

    CCrystal::CRYSTAL_DESC Desc{};
    Desc.vLook = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);
    Desc.strModelTag = PRO_MODEL_MOLOCH_CRYSTAL_A;
    Desc.pOwner = dynamic_cast<CMoloch*>(m_pMonster);

     _float4 fPos{};
     XMStoreFloat4(&fPos, m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS));

    _float fCount = 7.f;
    _float fOffset = 3.f;

    switch (m_iAnimIndex)
    {
        case CMoloch::MOLOCH_ATK_180_L:
        { 
            Regist_CollUpdate(20, 32);
            Secede_CollUpdate(20, 32);
        }

        break;
        case CMoloch::MOLOCH_ATK_FULL_180_R:
        {
            Regist_CollUpdate(18, 32);
            Secede_CollUpdate(18, 32);

            _float fCount = 9.f;
            _float fOffset = 4.5f;

            // 원뿔 형태로 만들어야 함
            if(22.f <= m_pModelCom->Get_CurAnimationTrackPosition() && !m_bSpawn)
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

        }
        break;
        
        case CMoloch::MOLOCH_ATK_180_R:
        {
            Regist_CollUpdate(22, 63);
            Secede_CollUpdate(22, 63);

           

        }
        break;
    }
    
    // 해당 각도가 되었을때 멈춰야 함
    // 크리스탈 생성하면 멈추게 하자
    if (!m_bSpawn)
    {
        Update_MonsterLook(fTimeDelta);
    }

    if(m_bAnimEnd)
    {
        Secede_PartCollUpdate();

        m_bRegisted = false;
        m_bSeceded = false;
        m_bSpawn = false;
        m_bTurned = false;
        return;
    }
}

void CMolochAttack_180::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)), -1.f);
}

void CMolochAttack_180::LateUpdate_State(_float fTimeDelta)
{

}

HRESULT CMolochAttack_180::Exit_State()
{
    Set_PreAnimation();
    Secede_PartCollUpdate();

    m_fElapseTime = 0.f;
    m_bAttack = false;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMolochAttack_180::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
    m_bAnimEnd = false;
}

void CMolochAttack_180::Update_Animation(_float fTimeDelta)
{
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);


    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));
}

void CMolochAttack_180::Set_CurAnimation()
{
    _bool bCheck = Check_PlayerLeft();

    if (bCheck)
    {
        m_iAnimIndex = CMoloch::MOLOCH_ATK_180_L;
    }

    else
    {
        _uint iNum = m_pGameInstance->Draw_RandomNum(2);

        if (1 == iNum)
        {
            m_iAnimIndex = CMoloch::MOLOCH_ATK_180_R;
        }
        else
        {
            m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_180_R;
        }
    }

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CMolochAttack_180::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack)
        return;

    else
    {
        m_fElapseTime += fTimeDelta;
    }
}

_bool CMolochAttack_180::Check_Attackable()
{
    Setting_PlayerInfo();

    _float fDegree = XMConvertToDegrees(Get_MonsterLookDot());

    if (m_fElapseTime >= m_fCoolTime && Check_Colls() && fDegree > 130.f)
            return true;

    return false;
}

_bool CMolochAttack_180::Check_Colls()
{
    Setting_PlayerInfo();

    _bool bColl = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
    _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

    if (!bColl && !bColl2)
        return true;

    return false;
}

CMolochAttack_180* CMolochAttack_180::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime)
{
    CMolochAttack_180* pInstance = new CMolochAttack_180(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;
    pInstance->m_fCoolTime = fCoolTime;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CMolochAttack_180::Free()
{
    __super::Free();
}
