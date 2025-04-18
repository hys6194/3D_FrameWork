#include "MolochAttack_Dash.h"
#include "GameInstance.h"
#include "Player.h"

#include "Moloch.h"
#include "Crystal.h"
#include "Body_Moloch.h"
//#include "Ghoul.h"
//#include "Body_Ghoul.h"

CMolochAttack_Dash::CMolochAttack_Dash(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base { pOwner, pAnimOwner }
{
}

HRESULT CMolochAttack_Dash::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    //m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_DASH_START;

    m_bAnimEnd = false;
    m_bAttack = true;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMolochAttack_Dash::PriorityUpdate_State(_float fTimeDelta)
{
    if(Check_Dead(fTimeDelta))                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  if (FAILED(Check_Dead(fTimeDelta)))
        return;

    _float fDegree = XMConvertToDegrees(Get_MonsterLookDot());

    switch (m_iAnimIndex)
    {
        case CMoloch::MOLOCH_ATK_FULL_DASH_START:
            if (m_bAnimEnd)
            {
                m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_DASH;
                m_pModelCom->Set_AnimationIndex(m_iAnimIndex, true);

                m_bAnimEnd = false;
            }

            return;
        case CMoloch::MOLOCH_ATK_FULL_DASH:
            {
                _bool bColl1 = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
                _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

                m_fTotalTime += fTimeDelta;

                // 여기에서 직진으로 엄청 빠르게 이동해야함
                m_pMonster->Get_Transform()->Set_Speed(75.f);
                m_pMonster->Get_Transform()->Go_Straight(fTimeDelta, 
                    static_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));



                if ((bColl1 && bColl2) || 
                    m_fTotalTime > 1.5f)
                {
                    m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_DASH_STRIKE_02;
                    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
                    m_fTotalTime = 0.f;
                }
                return;
            }
            
        case CMoloch::MOLOCH_ATK_FULL_DASH_STRIKE_02:
        {
            Regist_CollUpdate(0, 65);
            Secede_CollUpdate(0, 65);

            _bool bColl1 = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
            _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

            // 충돌했을때에만 생성하게
            // 근데 이러면 좀 애매한데 이 타이밍때 부딪쳐있어야만 하는거잖아 그냥 넘기자
            if (33.f <= m_pModelCom->Get_CurAnimationTrackPosition() &&
                !m_bSpawn)
            {
                m_bSpawn = true;
                Create_Crystals();
            }


            if (m_bAnimEnd)
            {
                m_iAnimIndex = CMoloch::MOLOCH_ATK_FULL_DASH_START;

                Secede_PartCollUpdate();

                m_bRegisted = false;
                m_bSeceded = false;
                m_fTotalTime = 0.f;
            }
        }
            return;

    }

    if (m_bAnimEnd)
    {
        Secede_PartCollUpdate();

        m_bRegisted = false;
        m_bSeceded = false;
        m_fTotalTime = 0.f;

        return;
    }


    // 처음 진입했을 때는 

    // 충돌체 업데이트 시간
    m_fTotalTime += m_pGameInstance->Get_TimeDelta(TIME60);
    
    // 애니메이션 돌아갈 때 히트 판정의 발동과 해제
    if (0.65f < m_fTotalTime && !m_bRegisted)
    {
        Regist_PartCollUpdate();
        m_bRegisted = true;
    }
    
    // 애니메이션의 중간에 이벤트를 발생해서 Update 등록 및 해제를 하고 싶은데 안되나
    if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() / 1.5f &&
        !m_bSeceded)
    {
        Secede_PartCollUpdate();
        m_bSeceded = true;
    }

}

void CMolochAttack_Dash::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);

    if (m_iAnimIndex != CMoloch::MOLOCH_ATK_FULL_DASH)
        m_pMonster->Get_Transform()->Set_Speed(7.5f);
}

void CMolochAttack_Dash::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMolochAttack_Dash::Exit_State()
{
    Set_PreAnimation();
    Secede_PartCollUpdate();

    m_fElapseTime = 0.f;
    m_bSpawn = false;
    m_bAttack = false;
    m_pMonster->Set_Attack(m_bAttack);

    return S_OK;
}

void CMolochAttack_Dash::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_ATTACK);
}

void CMolochAttack_Dash::Update_Animation(_float fTimeDelta)
{
    m_bAnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pAnimOwner);

    m_pMonster->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)), 0.5f);
}

void CMolochAttack_Dash::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex);
}

void CMolochAttack_Dash::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack /*|| m_fCoolTime이 원래 쿨타임과 같아지거나 커졌다면 */)
        return;
    else
    {
        m_fElapseTime += fTimeDelta;
    }
}

_bool CMolochAttack_Dash::Check_Attackable()
{
    // 쿨 다 돌면
    if (m_fElapseTime >= m_fCoolTime && Check_Colls())
        return true;

    return false;
}

_bool CMolochAttack_Dash::Check_Colls()
{
    Setting_PlayerInfo();

    _bool bColl1 = static_cast<CCollider*>(m_pMonster->Get_Component(COM_COLL_SPHERE))->Is_Coll();
    _bool bColl2 = static_cast<CCollider*>(m_pPlayer->Get_Component(COM_COLL_SPHERE))->Is_Coll();

    if (!bColl1 && !bColl2)
        return true;

    return false;
}

void CMolochAttack_Dash::Reset_Pattern()
{
    Secede_PartCollUpdate();

    m_bRegisted = false;
    m_bSeceded = false;
    m_fTotalTime = 0.f;
}

HRESULT CMolochAttack_Dash::Create_Crystals()
{
    CCrystal::CRYSTAL_DESC Desc{};
    Desc.vLook = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_LOOK);
    Desc.strModelTag = PRO_MODEL_MOLOCH_CRYSTAL_A;
    Desc.pOwner = dynamic_cast<CMoloch*>(m_pMonster);
    Desc.fLifeTime = 3.f;

    _float4 fMonsterPos{};
    XMStoreFloat4(&fMonsterPos, m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS));

    // 생성 개수
    mt19937 gen3(rand());

    uniform_int_distribution<> Count(16, 24);

    _uint iNum = Count(gen3);

    for (size_t i = 0; i < iNum; i++)
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

CMolochAttack_Dash* CMolochAttack_Dash::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime)
{
    CMolochAttack_Dash* pInstance = new CMolochAttack_Dash(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;
    pInstance->m_fCoolTime = fCoolTime;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CMolochAttack_Dash::Free()
{
    __super::Free();
}
