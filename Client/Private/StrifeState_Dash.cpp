#include "StrifeState_Dash.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"	
#include "Animation.h"

StrifeState_Dash::StrifeState_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
	:State{pDevice, pContext, pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT StrifeState_Dash::Enter_State()
{
	m_iState = dynamic_cast<Player*>(m_pOwner)->Get_PlayerState();

	Set_CurAnimation();

	return S_OK;
}

void StrifeState_Dash::PriorityUpdate_State(_float fTimeDelta)
{
	m_iKeyState = dynamic_cast<Player*>(m_pOwner)->Get_PlayerKeyState();

	_float f1 = m_pModelCom->Get_CurAnimationTrackPosition();
	_float f2 = m_pModelCom->Get_CurAnimationDuration() - 10.5f;


	if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() /3.f)
	{
		switch (m_iKeyState)
		{
		case Player::KEY_SHIFT:
			m_iState |= Player::STATE_DOUBLEDASH;
			m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH_END);
			dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(m_iState);

		default:
			break;
		}

	}

	if (true == m_bDashed)
		dynamic_cast<Player*>(m_pOwner)->Set_PlayerState(Player::STATE_IDLE);
}

void StrifeState_Dash::Update_State(_float fTimeDelta)
{
	Update_Animation(fTimeDelta);
}

void StrifeState_Dash::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT StrifeState_Dash::Exit_State()
{
	//Set_PreAnimation();
	if (true == m_bDashed && m_iState & Player::STATE_DOUBLEDASH)
		m_iState ^= Player::STATE_DOUBLEDASH;

	return S_OK;
}

void StrifeState_Dash::Set_CurAnimation()
{
	m_pModelCom = dynamic_cast<Body_Player*>(m_pAnimOwner)->Get_Model();

	m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH_BACK, false, false);
}

void StrifeState_Dash::Update_Animation(_float fTimeDelta)
{
	//if (0 != m_pModelCom->Get_PreAnimIndex()
	//	&& m_pModelCom->Get_Interpolate())
	//	m_pModelCom->Interpolate_Animation(0.2f);
	//else
	m_pModelCom->Play_RootAnimation(fTimeDelta);

	m_bDashed = m_pModelCom->Play_Animation(fTimeDelta);

}

void StrifeState_Dash::Set_PreAnimation()
{
	m_pModelCom->Reset_PreAnimation();

	m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::DASH_BACK);
}

StrifeState_Dash* StrifeState_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
{
	StrifeState_Dash* pInstance = new StrifeState_Dash( pDevice, pContext, pOwner, pAnimOwner);

	if (nullptr == pOwner)
	{
		MSG_BOX("Failed To Created : BodyState_Idle");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void StrifeState_Dash::Free()
{
	__super::Free();
}
