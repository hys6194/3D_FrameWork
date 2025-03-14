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
	Set_CurAnimation();

	m_iCheckDash++;

	return S_OK;
}

void CStrifeState_Dash::PriorityUpdate_State(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_LSHIFT))
		m_iCheckDash++;

	if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() /2.5f)
	{
		m_iKeyState = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerKeyState();

		if ((2 <= m_iCheckDash) && !m_bDashed)
		{	
			m_iState	 |= CPlayer::STATE_DOUBLEDASH;
			//dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(m_iState);
			m_bDashed = true;

			Set_LastDashAnimation();
		}
		 
		else if ((m_iKeyState & CPlayer::KEY_UP)   ||
				 (m_iKeyState & CPlayer::KEY_DOWN) ||
				 (m_iKeyState & CPlayer::KEY_LEFT) ||
				 (m_iKeyState & CPlayer::KEY_RIGHT))
		{
			m_iState |= CPlayer::STATE_RUN;
			dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_RUN);
			m_iCheckDash = 0;
			return;
		}

	}

	if (true == m_AnimEnd)
		dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_IDLE);
}

void CStrifeState_Dash::Update_State(_float fTimeDelta)
{
	Update_Animation(fTimeDelta);

	m_pOwner->Get_Transform()->Dash(m_pModelCom->Get_Delta());
}

void CStrifeState_Dash::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CStrifeState_Dash::Exit_State()
{
	//Set_PreAnimation();
	if (true == m_bDashed && m_iState & CPlayer::STATE_DOUBLEDASH)
		m_iState ^= CPlayer::STATE_DOUBLEDASH;

	m_AnimEnd = false;
	m_bDashed = false;

	return S_OK;
}

void CStrifeState_Dash::Set_CurAnimation()
{
	m_pModelCom = dynamic_cast<CBody_Player*>(m_pAnimOwner)->Get_Model();
	m_iKeyState = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerKeyState();

	Check_KeyInput();
	
}

void CStrifeState_Dash::Update_Animation(_float fTimeDelta)
{
	if (0 != m_pModelCom->Get_PreAnimIndex()
		&& m_pModelCom->Get_Interpolate())
		m_pModelCom->Interpolate_Animation(0.1f);
	else
		m_AnimEnd = m_pModelCom->Play_Animation(fTimeDelta, m_pOwner);

}

void CStrifeState_Dash::Set_PreAnimation()
{
	m_pModelCom->Reset_PreAnimation();

	m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::DASH_BACK);
}

void CStrifeState_Dash::Check_KeyInput()
{
	if ((m_iKeyState & CPlayer::KEY_UP) ||
		(m_iKeyState & CPlayer::KEY_DOWN) || 
		(m_iKeyState & CPlayer::KEY_LEFT) || 
		(m_iKeyState & CPlayer::KEY_RIGHT))
		m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH, false, true);

	else /*if(m_iKeyState == CPlayer::KEY_NONE)*/
		m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH_BACK, false, true);
}

void CStrifeState_Dash::Set_LastDashAnimation()
{
	if ((m_iKeyState & CPlayer::KEY_UP) ||
		(m_iKeyState & CPlayer::KEY_DOWN) ||
		(m_iKeyState & CPlayer::KEY_LEFT) ||
		(m_iKeyState & CPlayer::KEY_RIGHT))
		m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH_END, false, true);

	else /*if (m_iKeyState == CPlayer::KEY_SHIFT)*/
		m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH_BACKEND, false, true);

	m_iCheckDash = 0;
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
