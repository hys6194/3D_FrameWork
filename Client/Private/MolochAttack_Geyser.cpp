#include "MolochAttack_Geyser.h"
#include "GameInstance.h"
#include "Player.h"

#include "Moloch.h"
#include "Body_Moloch.h"
#include "Crystal.h"

CMolochAttack_Geyser::CMolochAttack_Geyser(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base { pOwner, pAnimOwner }
{
}

HRESULT CMolochAttack_Geyser::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    m_bAttack = true;
    m_pMonster->Set_Attack(m_bAttack);

    Set_Sound(TEXT("Moloch_atk_full_geyser_start"), SOUND_MOLOCH_GEYSER, 0.1f, false);
    Set_RandomSound(3, TEXT("Moloch_Geyser_"), SOUND_MOLOCH_GEYSER_VOICE, 0.1f, false);

    return S_OK;
}

void CMolochAttack_Geyser::PriorityUpdate_State(_float fTimeDelta)
{
    if(Check_Dead(fTimeDelta))                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  if (FAILED(Check_Dead(fTimeDelta)))
        return;

    switch (m_iAnimIndex)
    {
        case CMoloch::MOLOCH_ATK_FULL_GEYSER_START:
            if (m_bAnimEnd)
            {
                m_pGameInstance->Stop_Sound(SOUND_MOLOCH_GEYSER);
                m_pGameInstance->Stop_Sound(SOUND_MOLOCH_GEYSER_VOICE);

                m_pModelCom->Set_AnimationIndex(CMoloch::MOLOCH_ATK_FULL_GEYSER);
                m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_GEYSER;
                m_bAnimEnd = false;

                Set_RandomSound(3, TEXT("Moloch_atk_full_geyser_"), SOUND_MOLOCH_GEYSER, 0.1f, false);
                //Set_RandomSound(3, TEXT("Moloch_Geyser_"), SOUND_MOLOCH_GEYSER_VOICE, 0.2f, false);

            }
            break;
        case CMoloch::MOLOCH_ATK_FULL_GEYSER:
        {
            Regist_CollUpdate(0, 56);
            Secede_CollUpdate(0, 56);

            if(11.f <= m_pModelCom->Get_CurAnimationTrackPosition() && 
                !m_bSpawn)
            {
                m_bSpawn = true;
                Create_Crystals();
            }

            if (m_bAnimEnd)
            {
                m_pGameInstance->Stop_Sound(SOUND_MOLOCH_GEYSER);

                m_pModelCom->Set_AnimationIndex(CMoloch::MOLOCH_ATK_FULL_GEYSER_02);
                m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_GEYSER_02;
                m_bAnimEnd = false;
                m_bSpawn = false;

                Set_Sound(TEXT("Moloch_atk_full_geyser_jump"), SOUND_MOLOCH_GEYSER, 0.1f, false);
                Set_RandomSound(6, TEXT("Moloch_Laughs_"), SOUND_MOLOCH_GEYSER_VOICE, 0.15f, false);

            }
        }

            break;
        case CMoloch::MOLOCH_ATK_FULL_GEYSER_02:
            Regist_CollUpdate(0, 65);
            Secede_CollUpdate(0, 65);

            if (m_bAnimEnd)
            {
                m_pGameInstance->Stop_Sound(SOUND_MOLOCH_GEYSER);
                m_pGameInstance->Stop_Sound(SOUND_MOLOCH_GEYSER_VOICE);

                m_pMonster->Change_CurrentState(CMonster::STATE_SEARCH);
                m_bAnimEnd = false;
            }

            break;
    }

}

void CMolochAttack_Geyser::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMolochAttack_Geyser::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMolochAttack_Geyser::Exit_State()
{
    Set_PreAnimation();
    Secede_PartCollUpdate();

    m_fElapseTime = 0.f;

    m_bAttack = false;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMolochAttack_Geyser::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
}

void CMolochAttack_Geyser::Update_Animation(_float fTimeDelta)
{    
    if (0 != m_pModelCom->Get_PreAnimIndex()
          && m_pModelCom->Get_Interpolate())
        m_pModelCom->Interpolate_Animation(0.2f);
    else
        m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)), 0.5f);
}

void CMolochAttack_Geyser::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_GEYSER_START;


    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CMolochAttack_Geyser::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack)  return;

    else m_fElapseTime += fTimeDelta;
}

_bool CMolochAttack_Geyser::Check_Attackable()
{
    // 쿨 다 돌면
    if (m_fElapseTime >= m_fCoolTime && Check_Colls())
        return true;

    return false;
}

_bool CMolochAttack_Geyser::Check_Colls()
{
    Setting_PlayerInfo();

    _bool bColl1 = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
    _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

    if (!bColl1 && !bColl2)
        return true;

    return false;
}

HRESULT CMolochAttack_Geyser::Create_Crystals()
{
    CCrystal::CRYSTAL_DESC Desc{};
    Desc.vLook = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);
    Desc.strModelTag = PRO_MODEL_MOLOCH_CRYSTAL_A;
    Desc.pOwner = dynamic_cast<CMoloch*>(m_pMonster);
    Desc.fLifeTime = 4.f;

    _float4 fMonsterPos{};
    XMStoreFloat4(&fMonsterPos, m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS));
   
    // 생성 개수
    mt19937 gen3(rand());

    uniform_int_distribution<> Count(16, 24);

    _uint iNum = Count(gen3);

    for(size_t i = 0; i < iNum; i++)
    {
        random_device rand;
        mt19937 gen1(rand());
        mt19937 gen2(rand());

        float fDistance;
        fDistance = 30.f;

        // 거리
        uniform_real_distribution<> dis(-fDistance, fDistance);


        float fRandPosX = (_float)dis(gen1);
        float fRandPosZ = (_float)dis(gen2);

        Desc.vCrystalPos = XMVectorSet(fMonsterPos.x + fRandPosX,
            fMonsterPos.y,
            fMonsterPos.z + fRandPosZ,
            1.f);

        FAILED_CHECK_RETURN(m_pGameInstance->Add_GameObject(LEVEL_GAMEPLAY, PRO_OBJ_CRYSTAL,
            LEVEL_GAMEPLAY, Desc.strModelTag, &Desc), E_FAIL);
    }

    return S_OK;
}

CMolochAttack_Geyser* CMolochAttack_Geyser::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime)
{
    CMolochAttack_Geyser* pInstance = new CMolochAttack_Geyser(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;
    pInstance->m_fCoolTime = fCoolTime;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CMolochAttack_Geyser::Free()
{
    __super::Free();
}
