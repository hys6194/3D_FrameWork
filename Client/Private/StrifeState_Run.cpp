#include "StrifeState_Run.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"

#include "GameInstance.h"

StrifeState_Run::StrifeState_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
    : State{ pDevice , pContext, pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT StrifeState_Run::Enter_State()
{
    m_iState = dynamic_cast<Player*>(m_pOwner)->Get_PlayerState();
    m_iKeyState = dynamic_cast<Player*>(m_pOwner)->Get_PlayerKeyState();

    Set_CurAnimation();

    return S_OK;
}

void StrifeState_Run::PriorityUpdate_State(_float fTimeDelta)
{
    m_iKeyState = dynamic_cast<Player*>(m_pOwner)->Get_PlayerKeyState();

    if (m_iKeyState == Player::KEY_NONE)
    {
        dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(Player::STATE_IDLE);
    }

    else if(m_iKeyState == Player::KEY_SPACE)
        dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(Player::STATE_DASH);

    //
    //if (m_pGameInstance->Key_Pressing(DIK_DOWN))
    //{
    //    // Player의 Transform이어야 함
    //    m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(180.f));
    //    m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
    //}
    //
    //if (m_pGameInstance->Key_Pressing(DIK_LEFT))   
    //{
    //    m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(-90.f));
    //    m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
    //}
    //
    //if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
    //{
    //    m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(90.f));
    //    m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
    //}
    //
    //if (m_pGameInstance->Key_Pressing(DIK_UP))
    //{
    //    m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(0.f));
    //    m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);
    //  }
    //// 암것도 안누르면 Idle
    //else if(!m_pGameInstance->Key_Pressing(DIK_DOWN)  || !m_pGameInstance->Key_Pressing(DIK_UP)  ||
    //    !m_pGameInstance->Key_Pressing(DIK_LEFT)  || !m_pGameInstance->Key_Pressing(DIK_RIGHT) )
    //{
    //    m_iState ^= Player::STATE_RUN;
    //    m_iState |= Player::STATE_IDLE;
    //    dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(m_iState);
    //}
}

void StrifeState_Run::Update_State(_float fTimeDelta)
{   
    Update_Animation(fTimeDelta);
}

void StrifeState_Run::LateUpdate_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Get_Interpolate())
        m_pModelCom->Reset_PreAnimation();
}

HRESULT StrifeState_Run::Exit_State()
{
    Set_PreAnimation();
    return S_OK;
}

void StrifeState_Run::Set_CurAnimation()
{
    m_pModelCom = dynamic_cast<Body_Player*>(m_pAnimOwner)->Get_Model();

    m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::RUN, true);
}

void StrifeState_Run::Update_Animation(_float fTimeDelta)
{
    if (0 != m_pModelCom->Get_PreAnimIndex()
        && m_pModelCom->Get_Interpolate())
        m_pModelCom->Interpolate_Animation(0.2f);
    else
        m_pModelCom->Play_Animation(fTimeDelta);
}

void StrifeState_Run::Set_PreAnimation()
{
    m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::RUN);
}

StrifeState_Run* StrifeState_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
{
    StrifeState_Run* pInstance = new StrifeState_Run(pDevice, pContext, pOwner, pAnimOwner);

    if (nullptr == pOwner)
    {
        MSG_BOX("Failed To Created : StrifeState_Run");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void StrifeState_Run::Free()
{
    __super::Free();
}
