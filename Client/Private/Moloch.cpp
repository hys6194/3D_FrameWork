#include "Moloch.h"
#include "Monster.h"
#include "Status.h"
#include "GameInstance.h"

#include "Body_Moloch.h"
#include "Moloch_Sword.h"

#include "MonsterState_Attack.h"
#include "MonsterState_Search.h"
#include "MonsterState_Trace.h"
#include "MonsterState_Avoid.h"
#include "MonsterState_Idle.h"
#include "MonsterState_Dead.h"
#include "MonsterState_Hit.h"

CMoloch::CMoloch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CMonster { pDevice ,pContext }
{
}

CMoloch::CMoloch(const CMoloch& Prototype)
    : CMonster{ Prototype }
{
}

HRESULT CMoloch::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CMoloch::Initialize(void* pArg)
{
    MONSTER_DESC Desc{};

    Desc.bBoss = true;
    Desc.bWave = false;
    Desc.fSpeedPerSec = 5.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);
    Desc.iState = STATE_IDLE;
    Desc.strMonsterName = TEXT("_Moloch");
    m_iState = Desc.iState;

    // ÀÌ°Å ¹¹¿´Áö
    m_fDetectDistance = 12.f;
    m_fHitPersent = 0.5f;

    FAILED_CHECK_RETURN(__super::Initialize(&Desc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_PartObjects(), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);
    FAILED_CHECK_RETURN(Ready_States(), E_FAIL);


    m_pTransformCom->Set_State(CTransform::STATE_POS,
        XMVectorSet(16.459108f, 16.321875f, 146.170197f, 1.000000f));

    m_pFSMCom->Change_State(m_iState);

    return S_OK;
}

void CMoloch::Priority_Update(_float fTimeDelta)
{
     __super::Priority_Update(fTimeDelta);
    
    //if (m_pGameInstance->Key_Down(DIK_1))
    //    m_iState = STATE_HIT;
    //
    if (m_pGameInstance->Key_Down(DIK_2))
     m_iState = STATE_IDLE;
    
    if (m_pGameInstance->Key_Down(DIK_3))
     m_iState = STATE_DEAD;
    //
    //if (m_pGameInstance->Key_Down(DIK_4))
    //    m_iState = STATE_SEARCH;  
    //
    //if (m_pGameInstance->Key_Down(DIK_5))
    //    m_iState = STATE_AVOID;
    //
    //if (m_pGameInstance->Key_Down(DIK_6))
    //     m_iState = STATE_ATTACK;
    //
    //if (m_pGameInstance->Key_Down(DIK_7))
    //    m_iState = STATE_TRACE;
    
    
}

void CMoloch::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CMoloch::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CMoloch::Render()
{
    __super::Render();

    return S_OK;
}

HRESULT CMoloch::Ready_PartObjects()
{
    CBody_Moloch::BODY_MONSTER_DESC		BodyDesc{};
    BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pTargetState = &m_iState;
    
    FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_MOLOCH_BODY, PART_BODY, &BodyDesc), E_FAIL);

    CMoloch_Sword::MOLOCH_SWORD_DESC  GDesc2{};
    GDesc2.pHandMatrix = dynamic_cast<CBody_Moloch*>(m_vecParts[PART_BODY])->Get_f4SocketMatrix(SOCKET_MOLOCH_RIGHT_HAND);
    GDesc2.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    GDesc2.pTargetState = &m_iState;
    GDesc2.pOwner = this;
    FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_MOLOCH_SWORD, PART_RIGHT, &GDesc2), E_FAIL);

    return S_OK;
}

HRESULT CMoloch::Ready_States()
{
    CState* pState = nullptr;

    pState = CMonsterState_Idle::Create(this, m_vecParts[PART_BODY], MOLOCH_IDLE);
    m_pFSMCom->Add_State(CMonster::STATE_IDLE, pState);
    
    pState = CMonsterState_Hit::Create(this, m_vecParts[PART_BODY], MOLOCH_FULL_IMPACT);
    m_pFSMCom->Add_State(CMonster::STATE_HIT, pState);
    
    pState = CMonsterState_Search::Create(this, m_vecParts[PART_BODY], MOLOCH_TURN_90_L);
    m_pFSMCom->Add_State(CMonster::STATE_SEARCH, pState);
    
    pState = CMonsterState_Dead::Create(this, m_vecParts[PART_BODY], MOLOCH_FULL_IMPACT_STUN);
    m_pFSMCom->Add_State(CMonster::STATE_DEAD, pState);
    
    pState = CMonsterState_Attack::Create(this, m_vecParts[PART_BODY], MOLOCH_ATK_SWIP);
    m_pFSMCom->Add_State(CMonster::STATE_ATTACK, pState);
    
    pState = CMonsterState_Avoid::Create(this, m_vecParts[PART_BODY], MOLOCH_FULL_IDLE);
    m_pFSMCom->Add_State(CMonster::STATE_AVOID, pState);
    
    pState = CMonsterState_Trace::Create(this, m_vecParts[PART_BODY], MOLOCH_RUN_F);
    m_pFSMCom->Add_State(CMonster::STATE_TRACE, pState);

    return S_OK;
}

HRESULT CMoloch::Ready_Components()
{
    CNavigation::NAVIGATION_DESC		NaviDesc{};
    NaviDesc.iCellIndex = 446;

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_NAVI,
        reinterpret_cast<CComponent**>(&m_pNavigationCom), COM_NAVI, &NaviDesc), E_FAIL);

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_FSM,
        reinterpret_cast<CComponent**>(&m_pFSMCom), COM_FSM), E_FAIL);

    CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};
    ColliderDesc.vExtents = _float3(2.f, 7.f, 2.f);
    ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);
    ColliderDesc.eType = TYPE_AABB;
    ColliderDesc.strCollTag = Get_Name() + TEXT("_Body");
    ColliderDesc.iOption = COLL_OPT::OP_TARGET;
    ColliderDesc.pOwner = this;

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_AABB,
        reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL, &ColliderDesc), E_FAIL);
    
    CStatus::STATUS_DESC StatusDesc{};
    StatusDesc.iAttack = 2;
    StatusDesc.iHP = 50;
    StatusDesc.pOwner = this;

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_STATUS,
        reinterpret_cast<CComponent**>(&m_pStatusCom), COM_STATUS, &StatusDesc), E_FAIL);

    return S_OK;
}

CMoloch* CMoloch::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CMoloch* pInstance = new CMoloch(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Moloch");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CMoloch::Clone(void* pArg)
{
    CMoloch* pInstance = new CMoloch(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Moloch");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMoloch::Free()
{
    __super::Free();
    Safe_Release(m_pColliderCom);
}
