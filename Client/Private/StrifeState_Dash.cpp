#include "StrifeState_Dash.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"	
#include "Animation.h"

CStrifeState_Dash::CStrifeState_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
	:CState{pDevice, pContext, pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT CStrifeState_Dash::Enter_State()
{
	m_iState = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerState();

	Set_CurAnimation();

	return S_OK;
}

void CStrifeState_Dash::PriorityUpdate_State(_float fTimeDelta)
{
	m_iKeyState = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerKeyState();

		

	if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() /3.f)
	{
		switch (m_iKeyState)
		{
		case CPlayer::KEY_SHIFT:
			m_iState |= CPlayer::STATE_DOUBLEDASH;
			dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(m_iState);
			m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH_END);

		default:
			break;
		}

	}

	if (true == m_bDashed)
		dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_IDLE);
}

void CStrifeState_Dash::Update_State(_float fTimeDelta)
{
	Update_Animation(fTimeDelta);
}

void CStrifeState_Dash::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CStrifeState_Dash::Exit_State()
{
	//Set_PreAnimation();
	if (true == m_bDashed && m_iState & CPlayer::STATE_DOUBLEDASH)
		m_iState ^= CPlayer::STATE_DOUBLEDASH;

	return S_OK;
}

void CStrifeState_Dash::Set_CurAnimation()
{
	m_pModelCom = dynamic_cast<Body_Player*>(m_pAnimOwner)->Get_Model();

	m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH_BACK, false, false);
}

void CStrifeState_Dash::Update_Animation(_float fTimeDelta)
{
	//if (0 != m_pModelCom->Get_PreAnimIndex()
	//	&& m_pModelCom->Get_Interpolate())
	//	m_pModelCom->Interpolate_Animation(0.2f);
	//else
	//m_pModelCom->Play_RootAnimation(fTimeDelta);

	m_bDashed = m_pModelCom->Play_Animation(fTimeDelta, m_pOwner);

}

void CStrifeState_Dash::Set_PreAnimation()
{
	m_pModelCom->Reset_PreAnimation();

	m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::DASH_BACK);
}

CStrifeState_Dash* CStrifeState_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
{
	CStrifeState_Dash* pInstance = new CStrifeState_Dash( pDevice, pContext, pOwner, pAnimOwner);

	if (nullptr == pOwner)
	{
		MSG_BOX("Failed To Created : BodyState_Idle");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void CStrifeState_Dash::Free()
{
	__super::Free();
}
