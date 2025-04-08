#include "MonsterState_Trace.h"

#include "Player.h"
#include "Monster.h"
#include "Body_Ghoul.h"
#include "Ghoul.h"

CMonsterState_Trace::CMonsterState_Trace(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CMonsterState_Base{ pOwner, pAnimOwner }
{
}

HRESULT CMonsterState_Trace::Enter_State()
{ 
    Set_CurAnimation();
    Setting_PlayerInfo();

    m_iPreState = m_pMonster->Get_PreState();

    return S_OK;
}

void CMonsterState_Trace::PriorityUpdate_State(_float fTimeDelta)
{
    // 시발 여기에서 사라지는 거네?
    // 왜 사라지는 거니?
    // 이게 맞나? 차라리 Base에 그냥 함수로 만들어서 호출하는게 훨 나아보이기도 하고
    if (FAILED(Check_Dead(fTimeDelta)))
        return;

    if (FAILED(Check_Hit(fTimeDelta)))
        return;

    m_fUpdateTime += m_pGameInstance->Get_TimeDelta(TIME60);

    _float4 fTest11{};

    XMStoreFloat4(&fTest11, m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS));

    // 사라지기만 해봐 십련아 ㅋㅋ 디버깅 걸면 그만이야~
    if (_isnan(fTest11.x) ||
        _isnan(fTest11.y) ||
        _isnan(fTest11.z) ||
        _isnan(fTest11.w))
    {
        int a = 10;
    }

    // 1초마다 몬스터의 방향 설정 하려했는데 어색함
    if (1.f < m_fUpdateTime)
    {
        m_fUpdateTime = 0.f;
        Update_MonsterTurnSpeed(2.f);
        m_bTurned = false;

        _float4 fTest11{};

        XMStoreFloat4(&fTest11, m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS));

        // 사라지기만 해봐 십련아 ㅋㅋ 디버깅 걸면 그만이야~
        if (_isnan(fTest11.x) ||
            _isnan(fTest11.y) ||
            _isnan(fTest11.z) ||
            _isnan(fTest11.w))
        {
            int a = 10;
        }
    }



    // 만약 일직선이 아닐 경우
    if(!m_bTurned)
    {

        _bool bTurn = Update_MonsterLook(fTimeDelta);

        _float4 fTest11{};

        XMStoreFloat4(&fTest11, m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS));

        // 사라지기만 해봐 십련아 ㅋㅋ 디버깅 걸면 그만이야~
        if (_isnan(fTest11.x) ||
            _isnan(fTest11.y) ||
            _isnan(fTest11.z) ||
            _isnan(fTest11.w))
        {
            int a = 10;
        }

        if (bTurn)
            m_bTurned = bTurn;
    }

    _float4 fTest{};

    XMStoreFloat4(&fTest, m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS));

    // 사라지기만 해봐 십련아 ㅋㅋ 디버깅 걸면 그만이야~
    if (_isnan(fTest.x) ||
        _isnan(fTest.y) ||
        _isnan(fTest.z) ||
        _isnan(fTest.w))
    {
        int a = 10;
    }

    m_pMonster->Get_Transform()->Go_Straight(fTimeDelta, 
        dynamic_cast<CNavigation*>(m_pMonster->Get_Component(COM_NAVI)));

    _float4 fTest1{};

    XMStoreFloat4(&fTest1, m_pMonster->Get_Transform()->Get_State(CTransform::STATE_POS));

    // 사라지기만 해봐 십련아 ㅋㅋ 디버깅 걸면 그만이야~
    if (_isnan(fTest1.x) ||
        _isnan(fTest1.y) ||
        _isnan(fTest1.z) ||
        _isnan(fTest1.w))
    {
        int a = 10;
    }
}


void CMonsterState_Trace::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);


}

void CMonsterState_Trace::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMonsterState_Trace::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void CMonsterState_Trace::Set_PreAnimation()
{
    m_pModelCom->Reset_PreAnimation();
    m_pModelCom->Set_PreAnimation(m_iAnimIndex);
    m_pMonster->Set_PreState(CMonster::STATE_TRACE);

}

void CMonsterState_Trace::Update_Animation(_float fTimeDelta)
{
    __super::Update_Animation(fTimeDelta);
}

void CMonsterState_Trace::Set_CurAnimation()
{
    m_pModelCom = m_pBody->Get_Model();

    m_pModelCom->Set_AnimationIndex(m_iAnimIndex,true);
}

CMonsterState_Trace* CMonsterState_Trace::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex)
{
    CMonsterState_Trace* pInstance = new CMonsterState_Trace(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = AnimIndex;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance; 
}

void CMonsterState_Trace::Free()
{
    __super::Free();
}
