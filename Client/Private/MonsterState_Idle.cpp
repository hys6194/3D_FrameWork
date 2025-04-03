#include "MonsterState_Idle.h"

#include "Player.h"
#include "Monster.h"
#include "Body_Ghoul.h"
#include "Ghoul.h"

CMonsterState_Idle::CMonsterState_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base{pDevice, pContext, pOwner, pAnimOwner }
{
}

HRESULT CMonsterState_Idle::Enter_State()
{ 
    Set_CurAnimation();

    m_pPlayer = m_pGameInstance->Find_GameObject(LEVEL_GAMEPLAY, TEXT("Layer_Player"), TEXT("GameObject_Player"));

    return S_OK;
}

void CMonsterState_Idle::PriorityUpdate_State(_float fTimeDelta)
{
    // 이게 맞나? 차라리 Base에 그냥 함수로 만들어서 호출하는게 훨 나아보이기도 하고
    __super::PriorityUpdate_State(fTimeDelta);

    // 이 탐지거리라는게 플레이어와 몬스터의 위치 벡터를 구하고 그 거리내에 있어야 탐지거리가 되는 것인데 
    // 이는 어떻게 할 것이며 다른 몬스터의 상태에 접근하여 주변 몬스터에게 상태 변화를 어떻게 줄 것이냐
    // 또한 그 거리는 어떻게 에서 사용설정할 것인가?
    // 탐지거리는 어차피 Idle하고 다시는 안사용할 것 같으니 Idle에서 직접 쓰는 것으로 하자 
    // 몬스터마다 탐지거리는 달리 할 것인가?
    // 그것은 생각을 해봐야 함 
    // 어차피 탐지할 몬스터들은 보스를 제외한 나머지 간단한 몹들이므로 통일시켜 하자
    _vector vPos = m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS);
    _vector vPlayerPos = m_pPlayer->Get_Transform()->Get_State(CTransform::STATE_POS);

    _float fDistanace = XMVectorGetX(XMVector4Length(XMVectorSubtract(vPos, vPlayerPos)));


}


void CMonsterState_Idle::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMonsterState_Idle::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMonsterState_Idle::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void CMonsterState_Idle::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(CGhoul::STATE_IDLE);
}

void CMonsterState_Idle::Update_Animation(_float fTimeDelta)
{
    __super::Update_Animation(fTimeDelta);
}

void CMonsterState_Idle::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex, true);
}

CMonsterState_Idle* CMonsterState_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex)
{
    CMonsterState_Idle* pInstance = new CMonsterState_Idle(pDevice, pContext, pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = AnimIndex;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance; 
}

void CMonsterState_Idle::Free()
{
    __super::Free();
}
