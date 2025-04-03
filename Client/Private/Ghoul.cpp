#include "Ghoul.h"
#include "Monster.h"

#include "GameInstance.h"

#include "MonsterState_Idle.h"
#include "MonsterState_Hit.h"
#include "Body_Ghoul.h"

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
   
   //m_pGameInstance;


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
    m_pFSMCom->Add_State(STATE_IDLE, pState);   

    pState = CMonsterState_Hit::Create(this, m_vecParts[PART_BODY], CGhoul::GHOUL_IMPACT_F);
    m_pFSMCom->Add_State(STATE_HIT, pState);

    return S_OK;
}

HRESULT CGhoul::Ready_Components()
{
    __super::Ready_Components();

    CBounding_AABB::BOUNDING_AABB_DESC		ColliderDesc{};
    ColliderDesc.vExtents = _float3(1.f, 2.f, 1.f);
    ColliderDesc.vCenter = _float3(0.f, ColliderDesc.vExtents.y, 0.f);
    
    FAILED_CHECK_RETURN(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_COLL_AABB,
    	reinterpret_cast<CComponent**>(&m_pColliderCom), COM_COLL_AABB, &ColliderDesc), E_FAIL);
    
    
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
