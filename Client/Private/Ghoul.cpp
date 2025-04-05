#include "Ghoul.h"
#include "Monster.h"

#include "Body_Ghoul.h"
#include "GameInstance.h"

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

    lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Ghoul"));
    Desc.bBoss = false;
    Desc.bWave = false;
    Desc.fSpeedPerSec = 10.f;
    Desc.fRotationPerSec = XMConvertToRadians(90.f);
    Desc.iState = STATE_IDLE;
    m_iState = Desc.iState;

    FAILED_CHECK_RETURN(__super::Initialize(&Desc), E_FAIL);
    FAILED_CHECK_RETURN(Ready_PartObjects(), E_FAIL);
    FAILED_CHECK_RETURN(Ready_Components(), E_FAIL);
    FAILED_CHECK_RETURN(Ready_States(), E_FAIL);


    m_pFSMCom->Change_State(m_iState);

    return S_OK;
}

void CGhoul::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

   if (m_pGameInstance->Key_Down(DIK_1))
       m_iState = STATE_HIT;
   
   if (m_pGameInstance->Key_Down(DIK_2))
	   m_iState = STATE_IDLE;

   if (m_pGameInstance->Key_Down(DIK_3))
	   m_iState = STATE_DEAD;

   if (m_pGameInstance->Key_Down(DIK_4))
       m_iState = STATE_SEARCH;  

   if (m_pGameInstance->Key_Down(DIK_5))
       m_iState = STATE_AVOID;

   if (m_pGameInstance->Key_Down(DIK_6))
       m_iState = STATE_ATTACK;

   if (m_pGameInstance->Key_Down(DIK_7))
       m_iState = STATE_TRACE;

   if (m_pGameInstance->Key_Down(DIK_8))
   {
       m_bHit = true;
       m_bRec = true;
   }
   else
       m_bHit = false;

   //m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSet(1.61f, 2.96f, 34.18f, 1.00f));

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
    
    FAILED_CHECK_RETURN(__super::Add_PartObject(LEVEL_GAMEPLAY, PRO_OBJ_GHOUL_BODY, PART_BODY, &BodyDesc), E_FAIL);


    return S_OK;
}

HRESULT CGhoul::Ready_States()
{
    // 이 구조 좋다
    // 차라리 출력해줘야 할 애니메이션을 세팅하는게 훨 낫다
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

    pState = CMonsterState_Avoid::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_EVADE_LEFT);
    m_pFSMCom->Add_State(CMonster::STATE_AVOID, pState);

    pState = CMonsterState_Trace::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_RUN_F);
    m_pFSMCom->Add_State(CMonster::STATE_TRACE, pState);

    return S_OK;
}

HRESULT CGhoul::Ready_Components()
{
    __super::Ready_Components();

    CBounding_OBB::BOUNDING_OBB_DESC		ColliderDesc{};
    ColliderDesc.vExtents = _float3(1.f, 2.f, 1.f);
    ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);
    ColliderDesc.vRotation = _float3(0.f, 0.f, 0.f);
    
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_OBB,
    	reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL_OBB, &ColliderDesc), E_FAIL);
    
    
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
    Safe_Release(m_pColliderCom);

}
