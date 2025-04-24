#include "StrifeState_Dash.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"	
#include "Animation.h"

CStrifeState_Dash::CStrifeState_Dash(CGameObject* pOwner, CGameObject* pAnimOwner)
	:CState{ pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT CStrifeState_Dash::Enter_State()
{
	Set_CurAnimation();

	m_iCheckDash++;

	_uint iNum = m_pGameInstance->Draw_RandomNum(3);
	wstring strSoundName = TEXT("Strife_dash_1_") + to_wstring(iNum);
	m_pGameInstance->Play_Sound(strSoundName, SOUND_PLAYER_DASH,0.2f);

	return S_OK;
}

void CStrifeState_Dash::PriorityUpdate_State(_float fTimeDelta)
{
	if (m_pGameInstance->Key_Down(DIK_LSHIFT))
		m_iCheckDash++;

	if (m_pModelCom->Get_CurAnimationTrackPosition() >= m_pModelCom->Get_CurAnimationDuration() / 2.5f)
	{
		m_iKeyState = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerKeyState();

		if ((2 <= m_iCheckDash) && !m_bDashed)
		{	
			m_iState	 |= CPlayer::STATE_DOUBLEDASH;
			m_bDashed = true;

			m_pGameInstance->Stop_Sound(SOUND_PLAYER_DASH);
			_uint iNum = m_pGameInstance->Draw_RandomNum(2);
			wstring strSoundName = TEXT("Strife_dash_double_") + to_wstring(iNum);
			m_pGameInstance->Play_Sound(strSoundName, SOUND_PLAYER_DASH, 0.2f);

			Set_LastDashAnimation();
			Set_Player_Direction();
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

	m_pOwner->Get_Transform()->Dash(m_pModelCom->Get_Delta(), dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
}

void CStrifeState_Dash::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CStrifeState_Dash::Exit_State()
{
	if (true == m_bDashed && m_iState & CPlayer::STATE_DOUBLEDASH)
		m_iState ^= CPlayer::STATE_DOUBLEDASH;

	m_AnimEnd = false;
	m_bDashed = false;

	m_pGameInstance->Stop_Sound(SOUND_PLAYER_DASH);

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

	else 
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

void CStrifeState_Dash::Set_Player_Direction()
{
	switch (m_iKeyState)
	{
	case (CPlayer::KEY_DOWN | CPlayer::KEY_LEFT):
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(-135.f));
		break;

	case (CPlayer::KEY_UP | CPlayer::KEY_LEFT):
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(-45.f));
		break;

	case(CPlayer::KEY_UP | CPlayer::KEY_RIGHT):
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(45.f));
		break;

	case(CPlayer::KEY_RIGHT | CPlayer::KEY_DOWN):
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(135.f));
		break;

	case CPlayer::KEY_DOWN:
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(180.f));
		break;

	case CPlayer::KEY_LEFT:
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(-90.f));
		break;

	case CPlayer::KEY_RIGHT:
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(90.f));
		break;

	case CPlayer::KEY_UP:
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, XMConvertToRadians(0.f));
		break;
	}
}

CStrifeState_Dash* CStrifeState_Dash::Create(CGameObject* pOwner, CGameObject* pAnimOwner)
{
	CStrifeState_Dash* pInstance = new CStrifeState_Dash(pOwner, pAnimOwner);

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
