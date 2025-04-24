#include "StrifeState_Shoot.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"	

#include "GameInstance.h"

CStrifeState_Shoot::CStrifeState_Shoot(CGameObject* pOwner, CGameObject* pAnimOwner)
	:CState{ pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT CStrifeState_Shoot::Enter_State()
{
	Set_CurAnimation();

	return S_OK;
}

void CStrifeState_Shoot::PriorityUpdate_State(_float fTimeDelta)
{
	m_iKeyState = dynamic_cast<CPlayer*>(m_pOwner)->Get_PlayerKeyState();

	// ÃÑÀ» ½ð´Ù
	if (m_iKeyState & CPlayer::KEY_LB)
	{
		Player_LookSet(fTimeDelta);

		Player_ShootMove(fTimeDelta);

		Apply_ShootAnimation();
		
	}
	// ¾È ½ð´Ù
	else
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_IDLE);
		m_pGameInstance->Stop_Sound(SOUND_PLAYER_MOVE);
	}

}

void CStrifeState_Shoot::Update_State(_float fTimeDelta)
{
	Update_Animation(fTimeDelta);
}

void CStrifeState_Shoot::LateUpdate_State(_float fTimeDelta)
{

}

HRESULT CStrifeState_Shoot::Exit_State()
{
	Set_PreAnimation();


	m_pGameInstance->Stop_Sound(SOUND_PLAYER_MOVE);

	return S_OK;
}

void CStrifeState_Shoot::Set_PreAnimation()
{
	m_pModelCom->Reset_PreAnimation();
	m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::AIM_IDLE);
}

void CStrifeState_Shoot::Update_Animation(_float fTimeDelta)
{
	if (m_pModelCom->Get_Interpolate())
		m_pModelCom->Interpolate_Animation(0.2f);
	else
		m_pModelCom->Play_Animation(fTimeDelta);
}

void CStrifeState_Shoot::Set_CurAnimation()
{
	m_pModelCom = dynamic_cast<CBody_Player*>(m_pAnimOwner)->Get_Model();
	
	m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::AIM_IDLE, true, false);
}

void CStrifeState_Shoot::Player_ShootMove(_float fTimeDelta)
{
	if (9 < m_pModelCom->Get_CurKeyFrameIndex() ||
		11 > m_pModelCom->Get_CurKeyFrameIndex())
	{
		_uint iNum = m_pGameInstance->Draw_RandomNum(14);
		wstring strSoundName = TEXT("Strife_foot_") + to_wstring(iNum);
		m_pGameInstance->Play_Sound(strSoundName, SOUND_PLAYER_MOVE, 0.1f, true);
	}

    switch (m_iKeyState)
    {
	case CPlayer::KEY_LEFT | CPlayer::KEY_UP | CPlayer::KEY_LB:
		m_pOwner->Get_Transform()->Move_Left_Up(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
		break;

	case CPlayer::KEY_RIGHT | CPlayer::KEY_DOWN | CPlayer::KEY_LB:
		m_pOwner->Get_Transform()->Move_Right_Down(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
		break;

	case CPlayer::KEY_LEFT | CPlayer::KEY_DOWN | CPlayer::KEY_LB:
		m_pOwner->Get_Transform()->Move_Left_Down(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
		break;

	case CPlayer::KEY_RIGHT | CPlayer::KEY_UP | CPlayer::KEY_LB:
		m_pOwner->Get_Transform()->Move_Right_Up(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
		break;

    case CPlayer::KEY_DOWN | CPlayer::KEY_LB:
        m_pOwner->Get_Transform()->Move_Backward(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case CPlayer::KEY_LEFT | CPlayer::KEY_LB:
        m_pOwner->Get_Transform()->Move_Left(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case CPlayer::KEY_RIGHT | CPlayer::KEY_LB:
        m_pOwner->Get_Transform()->Move_Right(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;

    case CPlayer::KEY_UP | CPlayer::KEY_LB:
        m_pOwner->Get_Transform()->Move_Straight(fTimeDelta, dynamic_cast<CNavigation*>(m_pOwner->Get_Component(COM_NAVI)));
        break;
    }
}

void CStrifeState_Shoot::Player_LookSet(_float fTimeDelta)
{
	//_vector vOffset = XMVectorSet(0.f, 2.f, 0.f, 0.f);
	//
	_vector vPos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_POS);

	_float4 fPlayerPos{};
	XMStoreFloat4(&fPlayerPos, vPos);

	_vector vWin = *m_pGameInstance->Get_PlayerViewPortPos();
	//Get_RayDirCoords
	
	_vector vzero{ 0.f,1.f,0.f,0.f };
	_vector vMouse = XMVector4Normalize(vWin);
	
	_vector vResult = XMVector4Dot(vWin, vzero);
	
	_float fDot = XMVectorGetW(vResult);
	
	_float fX = XMVectorGetX(vWin);
	
	_float lengthA = XMVectorGetX(XMVector3Length(vWin));
	_float lengthB = XMVectorGetX(XMVector3Length(vzero));
	
	_float cosTheta = fDot / (lengthA * lengthB);
	
	if (fX >= 0)
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, acosf(cosTheta));
	else
		m_pOwner->Get_Transform()->Rotation(AXIS_Y, -acosf(cosTheta));


}

void CStrifeState_Shoot::Apply_ShootAnimation()
{
	if (m_iKeyState & CPlayer::KEY_DOWN || m_iKeyState & CPlayer::KEY_UP || m_iKeyState & CPlayer::KEY_LEFT || m_iKeyState & CPlayer::KEY_RIGHT)
		m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::AIM_WALK, true, false);

	else if (!(m_iKeyState & CPlayer::KEY_DOWN) && !(m_iKeyState & CPlayer::KEY_UP) && !(m_iKeyState & CPlayer::KEY_LEFT) && !(m_iKeyState & CPlayer::KEY_RIGHT))
		m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::AIM_IDLE, true, false);
}

CStrifeState_Shoot* CStrifeState_Shoot::Create(CGameObject* pOwner, CGameObject* pAnimOwner)
{
	return new CStrifeState_Shoot(pOwner, pAnimOwner);
}

void CStrifeState_Shoot::Free()
{
	__super::Free();
}
