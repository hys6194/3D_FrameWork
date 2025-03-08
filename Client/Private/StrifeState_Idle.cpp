#include "StrifeState_Idle.h"
#include "Body_Player.h"
#include "Model.h"
#include "Player.h"

StrifeState_Idle::StrifeState_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
    : State{ pDevice , pContext, pOwner, pAnimOwner }
{
}

HRESULT StrifeState_Idle::Enter_State()
{
    m_pOwner = dynamic_cast<Player*>(m_pOwner);
    m_pAnimOwner = dynamic_cast<Body_Player*>(m_pAnimOwner);

    Set_CurAnimation();
    return S_OK;
}

void StrifeState_Idle::PriorityUpdate_State(_float fTimeDelta)
{
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		if (m_iState & Player::STATE_IDLE)
			m_iState ^= Player::STATE_IDLE;
		m_iState |= Player::STATE_DASH;

        dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(m_iState);
	}

    if (false == dynamic_cast<Player*>(m_pOwner)->Get_PlayerMove())
    {
        dynamic_cast<Player*>(m_pOwner)->Set_PlayerMove(false);
    }

	// 아무 키나 눌렀을 때,
    else if ((GetAsyncKeyState(VK_DOWN) & 0x8000) || (GetAsyncKeyState(VK_LEFT) & 0x8000) ||(GetAsyncKeyState(VK_RIGHT) & 0x8000) ||(GetAsyncKeyState(VK_UP) & 0x8000))
	{
        if(false == dynamic_cast<Player*>(m_pOwner)->Get_PlayerMove())
            dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(m_iState);


		else if (m_iState & Player::STATE_IDLE)
			m_iState ^= Player::STATE_IDLE;
		m_iState |= Player::STATE_RUN;
        dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(m_iState);

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

    m_pModelCom->Set_Interpolate(true);
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
