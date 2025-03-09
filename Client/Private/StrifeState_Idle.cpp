#include "StrifeState_Idle.h"
#include "Body_Player.h"
#include "Model.h"
#include "Player.h"

StrifeState_Idle::StrifeState_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
    : State{ pDevice , pContext, pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT StrifeState_Idle::Enter_State()
{
    m_iState = dynamic_cast<Player*>(m_pOwner)->Get_PlayerState();

    
    Set_CurAnimation();

    return S_OK;
}

void StrifeState_Idle::PriorityUpdate_State(_float fTimeDelta)
{
    m_iKeyState = dynamic_cast<Player*>(m_pOwner)->Get_PlayerKeyState();

	if (m_iKeyState & Player::KEY_SHIFT)
	{
        dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(Player::STATE_DASH);
	}

	// 아무 키나 눌렀을 때,
    //else if (m_iKeyState & Player::KEY_UP || m_iKeyState & Player::KEY_LEFT || m_iKeyState & Player::KEY_DOWN || m_iKeyState & Player::KEY_RIGHT)
    else if (m_iKeyState & Player::KEY_UP ||
             m_iKeyState & Player::KEY_DOWN ||
             m_iKeyState & Player::KEY_LEFT ||
             m_iKeyState & Player::KEY_RIGHT)
	{
        dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(Player::STATE_RUN);
        //Check_KeyInput();
	}

    
}

void StrifeState_Idle::Update_State(_float fTimeDelta)
{   
    Update_Animation(fTimeDelta);
}

void StrifeState_Idle::LateUpdate_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Get_Interpolate())
        m_pModelCom->Reset_PreAnimation();
}

HRESULT StrifeState_Idle::Exit_State()
{
    Set_PreAnimation();

    return S_OK;
}

void StrifeState_Idle::Set_CurAnimation()
{
    m_pModelCom = dynamic_cast<Body_Player*>(m_pAnimOwner)->Get_Model();

    m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::IDLE, true);
}

void StrifeState_Idle::Update_Animation(_float fTimeDelta)
{
    if (0 != m_pModelCom->Get_PreAnimIndex()
        && m_pModelCom->Get_Interpolate())
        m_pModelCom->Interpolate_Animation(0.2f);
    else
        m_pModelCom->Play_Animation(fTimeDelta);
}

void StrifeState_Idle::Set_PreAnimation()
{
    m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::IDLE);
}

void StrifeState_Idle::Check_KeyInput()
{
    if (m_pGameInstance->Key_Pressing(DIK_DOWN))
    {
        dynamic_cast<Player*>(m_pOwner)->Insert_KeyState(Player::KEY_DOWN);
    }
    else if (!m_pGameInstance->Key_Pressing(DIK_DOWN))
    {
        dynamic_cast<Player*>(m_pOwner)->Delete_KeyState(!Player::KEY_DOWN);
    }

    if (m_pGameInstance->Key_Pressing(DIK_UP))
    {
        dynamic_cast<Player*>(m_pOwner)->Insert_KeyState(Player::KEY_UP);
    }
    else if (!m_pGameInstance->Key_Pressing(DIK_UP))
    {
        dynamic_cast<Player*>(m_pOwner)->Delete_KeyState(!Player::KEY_UP);
    }

    if (m_pGameInstance->Key_Pressing(DIK_LEFT))
    {
        dynamic_cast<Player*>(m_pOwner)->Insert_KeyState(Player::KEY_LEFT);
    }
    else if (!m_pGameInstance->Key_Pressing(DIK_LEFT))
    {
        dynamic_cast<Player*>(m_pOwner)->Delete_KeyState(!Player::KEY_LEFT);
    }

    if (m_pGameInstance->Key_Pressing(DIK_RIGHT))
    {
        dynamic_cast<Player*>(m_pOwner)->Insert_KeyState(Player::KEY_RIGHT);
    }
    else if (!m_pGameInstance->Key_Pressing(DIK_RIGHT))
    {
        dynamic_cast<Player*>(m_pOwner)->Delete_KeyState(!Player::KEY_RIGHT);
    }
}

StrifeState_Idle* StrifeState_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
{
    StrifeState_Idle* pInstance = new StrifeState_Idle(pDevice, pContext, pOwner, pAnimOwner);

    if (nullptr == pOwner)
    {
        MSG_BOX("Failed To Created : StrifeState_Idle");
        Safe_Release(pInstance);
        return nullptr;
    }

    //pInstance->Set_Owner(pOwner);
    //Safe_AddRef(m_pOwner);

    return pInstance;
}

void StrifeState_Idle::Free()
{
    __super::Free();
}
