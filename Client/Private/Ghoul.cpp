#include "Ghoul.h"
#include "Monster.h"
#include "Status.h"
#include "Attack.h"

#include "Fist_Left.h"
#include "Fist_Right.h"
#include "Body_Ghoul.h"
#include "GameInstance.h"

#include "GhoulAttack_Leaf.h"
#include "GhoulAttack_Flurry.h"
#include "GhoulAttack_DoubleSwipe.h"

#include "MonsterState_LookOut.h"
#include "MonsterState_Attack.h"
#include "MonsterState_Search.h"
#include "MonsterState_Trace.h"
#include "MonsterState_Avoid.h"
#include "MonsterState_Idle.h"
#include "MonsterState_Dead.h"
#include "MonsterState_Hit.h"

CGhoul::CGhoul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CMonster { pDevice ,pContext }
{
}

CGhoul::CGhoul(const CGhoul& Prototype)
    : CMonster{ Prototype }
{
}

HRESULT CGhoul::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGhoul::Initialize(void* pArg)
{
    MONSTER_DESC Desc{};

    Desc.bBoss = false;
    Desc.bWave = false;
    Desc.fSpeedPerSec = 10.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);
    Desc.iState = STATE_IDLE;
    Desc.strMonsterName = TEXT("_Ghoul");
    m_iState = Desc.iState;


    m_fDetectDistance = 15.f;
    m_fAttackDistance = 4.f;
    m_fHitPersent = 15.f;
    m_fAttackCoolTime = 3.f;

    FAILED_CHECK_RETURN(__super::Initialize(&Desc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_PartObjects(), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);
    FAILED_CHECK_RETURN(Ready_States(), E_FAIL);

    // 잠깐 랜덤 생성시키기
    m_pTransformCom->Set_State(CTransform::STATE_POS,
        XMVectorSet(m_pGameInstance->Random(0.f, 10.f), 2.f, m_pGameInstance->Random(0.f, 10.f), 1.f));

    m_pFSMCom->Change_State(m_iState);

    return S_OK;
}

void CGhoul::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);
}

void CGhoul::Update(_float fTimeDelta)
{
    __super::Update(fTimeDelta);
}

void CGhoul::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);
}

HRESULT CGhoul::Render()
{
    __super::Render();


    return S_OK;
}

HRESULT CGhoul::Ready_PartObjects()
{
    // 손에 어떻게 충돌체를 어떻게 부착해야 함?

    CBody_Ghoul::BODY_MONSTER_DESC		BodyDesc{};
    BodyDesc.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    BodyDesc.pTargetState = &m_iState;
    BodyDesc.pOwner = this;
    
    FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_GHOUL_BODY, PART_BODY, &BodyDesc), E_FAIL);

    CFist_Left::FIST_LEFT_DESC  FDesc1{};
    FDesc1.pHandMatrix = dynamic_cast<CBody_Ghoul*>(m_vecParts[PART_BODY])->Get_f4SocketMatrix(SOCKET_GHOUL_LEFT_HAND);
    FDesc1.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    FDesc1.pTargetState = &m_iState;
    FDesc1.pOwner = this;

    FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_L_FIST, PART_LEFT, &FDesc1), E_FAIL);

    CFist_Right::FIST_RIGHT_DESC  FDesc2{};
    FDesc2.pHandMatrix = dynamic_cast<CBody_Ghoul*>(m_vecParts[PART_BODY])->Get_f4SocketMatrix(SOCKET_GHOUL_RIGHT_HAND);
    FDesc2.pParentMatrix = m_pTransformCom->Get_WorldMatrix_Ptr();
    FDesc2.pTargetState = &m_iState;
    FDesc2.pOwner = this;
    FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_R_FIST, PART_RIGHT, &FDesc2), E_FAIL);


    return S_OK;
}

HRESULT CGhoul::Ready_States()
{
    CState* pState = nullptr;

    pState = CMonsterState_Idle::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_IDLE);
    m_pFSMCom->Add_State(CMonster::STATE_IDLE, pState);

    pState = CMonsterState_Hit::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_IMPACT_F);
    m_pFSMCom->Add_State(CMonster::STATE_HIT, pState);

    pState = CMonsterState_Search::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_TURN90_L);
    m_pFSMCom->Add_State(CMonster::STATE_SEARCH, pState);

    pState = CMonsterState_Dead::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_DEATH);
    m_pFSMCom->Add_State(CMonster::STATE_DEAD, pState);

    pState = CMonsterState_Attack::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_ATK_FLURRY);
    m_pFSMCom->Add_State(CMonster::STATE_ATTACK, pState);

    pState = CGhoulAttack_Leaf::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_ATK_LEAP, 5.f);
    m_pAttackCom->Regist_AttackPattern(GHOUL_ATK_LEAP, static_cast<CAttack_Base*>(pState));

    pState = CGhoulAttack_Flurry::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_ATK_FLURRY, 5.f);
    m_pAttackCom->Regist_AttackPattern(GHOUL_ATK_FLURRY, static_cast<CAttack_Base*>(pState));

    pState = CGhoulAttack_DoubleSwipe::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_ATK_DOUBLESWIPE, 5.f);
    m_pAttackCom->Regist_AttackPattern(GHOUL_ATK_DOUBLESWIPE, static_cast<CAttack_Base*>(pState));

    pState = CMonsterState_LookOut::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_WALK_L);
    m_pFSMCom->Add_State(CMonster::STATE_LOOKOUT, pState);

    pState = CMonsterState_Trace::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_RUN_F);
    m_pFSMCom->Add_State(CMonster::STATE_TRACE, pState);

    return S_OK;
}

HRESULT CGhoul::Ready_Components()
{
    __super::Ready_Components();

    CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};
    ColliderDesc.vExtents = _float3(1.f, 2.f, 1.f);
    ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);
    //ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
    //ColliderDesc.strCollTag = Get_Name() + TEXT("_Body ") + std::to_wstring(m_iIndex);
    ColliderDesc.eType = TYPE_AABB;
    ColliderDesc.strCollTag = Get_Name() + TEXT("_Body");
    ColliderDesc.iOption = COLL_OPT::OP_TARGET;
    ColliderDesc.pOwner = this;
    
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_AABB,
    	reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_AABB]), COM_COLL_AABB, &ColliderDesc), E_FAIL);
    
    CBounding_Sphere::BOUNDING_SPHERE_DESC		SphereDesc{};
    SphereDesc.fRadius = 1.f;
    SphereDesc.vCenter = _float3(0.f, 0.f, 0.f);
    SphereDesc.strCollTag = Get_Name() + TEXT("_Body_Detect");
    SphereDesc.iOption = COLL_OPT::OP_DETECT;
    SphereDesc.eType = TYPE::TYPE_SPHERE;
    SphereDesc.pOwner = this;

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_SPHERE,
        reinterpret_cast<CComponent**>(&m_pColliderCom[COLL_SPHERE]), COM_COLL_SPHERE, &SphereDesc), E_FAIL);


    CStatus::STATUS_DESC StatusDesc{};
    StatusDesc.iAttack = 2;
    StatusDesc.iHP = 100;
    StatusDesc.pOwner = this;

    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_STATUS,
        reinterpret_cast<CComponent**>(&m_pStatusCom), COM_STATUS, &StatusDesc), E_FAIL);


    
    return S_OK;
}

CGhoul* CGhoul::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CGhoul* pInstance = new CGhoul(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Created : Ghoul");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CGameObject* CGhoul::Clone(void* pArg)
{
    CGhoul* pInstance = new CGhoul(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Ghoul");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CGhoul::Free()
{
    __super::Free();

#ifdef _DEBUG
    for (size_t i = 0; i < TYPE_END; i++)
    {
        Safe_Release(m_pColliderCom[i]);
    }

#endif

}
