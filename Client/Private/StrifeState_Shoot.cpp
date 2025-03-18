#include "StrifeState_Shoot.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"	

#include "GameInstance.h"

CStrifeState_Shoot::CStrifeState_Shoot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
	:CState{pDevice, pContext, pOwner, pAnimOwner, m_pGameInstance }
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

	else if (m_iKeyState & CPlayer::KEY_RB)
	{
		// Ä«¸Þ¶ó ÀÌµ¿ÇÏ¸é¼­ ÃÑ ½î´Â °É·Î
	}

	// ¾È ½ð´Ù
	else
	{
		dynamic_cast<CPlayer*>(m_pOwner)->Set_PlayerState(CPlayer::STATE_IDLE);
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
    switch (m_iKeyState)
    {
	//case CPlayer::KEY_DOWN | CPlayer::KEY_LEFT | CPlayer::KEY_LB:
    //    dynamic_cast<CPlayer*>(m_pOwner)->Get_Transform()->Go_Straight(fTimeDelta);
    //    break;
	//
    //case CPlayer::KEY_UP | CPlayer::KEY_LEFT | CPlayer::KEY_LB:
    //    dynamic_cast<CPlayer*>(m_pOwner)->Get_Transform()->Go_Straight(fTimeDelta);
    //    break;
	//
    //case CPlayer::KEY_UP | CPlayer::KEY_RIGHT | CPlayer::KEY_LB:
    //    dynamic_cast<CPlayer*>(m_pOwner)->Get_Transform()->Go_Straight(fTimeDelta);
    //    break;
	//
    //case CPlayer::KEY_RIGHT | CPlayer::KEY_DOWN | CPlayer::KEY_LB:
    //    dynamic_cast<CPlayer*>(m_pOwner)->Get_Transform()->Go_Straight(fTimeDelta);
    //    break;

    case CPlayer::KEY_DOWN | CPlayer::KEY_LB:
        dynamic_cast<CPlayer*>(m_pOwner)->Get_Transform()->Go_Backward(fTimeDelta);
        break;

    case CPlayer::KEY_LEFT | CPlayer::KEY_LB:
        dynamic_cast<CPlayer*>(m_pOwner)->Get_Transform()->Go_Left(fTimeDelta);
        break;

    case CPlayer::KEY_RIGHT | CPlayer::KEY_LB:
        dynamic_cast<CPlayer*>(m_pOwner)->Get_Transform()->Go_Right(fTimeDelta);
        break;

    case CPlayer::KEY_UP | CPlayer::KEY_LB:
        dynamic_cast<CPlayer*>(m_pOwner)->Get_Transform()->Go_Straight(fTimeDelta);
        break;
    }
}

void CStrifeState_Shoot::Player_LookSet(_float fTimeDelta)
{
	_vector vPos = m_pOwner->Get_Transform()->Get_State(CTransform::STATE_POS);
	_vector vMouse = *m_pGameInstance->Picking_WindowsCoord(
		dynamic_cast<CPlayer*>(m_pOwner)->Get_Transform()->Get_WorldMatrix_Ptr());

	_vector vzero{ 0.f,1.f,0.f,0.f };
	vMouse = XMVector4Normalize(vMouse);

	_vector vResult = XMVector4Dot(vMouse, vzero);

	_float fDot = XMVectorGetW(vResult);

	_float fX = XMVectorGetX(vMouse);

	_float lengthA = XMVectorGetX(XMVector3Length(vMouse));
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

CStrifeState_Shoot* CStrifeState_Shoot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner)
{
	//CStrifeState_Shoot* pInstance = new CStrifeState_Shoot(pDevice, pContext, pOwner, pAnimOwner);
	//
	//if (nullptr == pOwner)
	//{
	//	MSG_BOX("Failed To Created : StrifeState_Shoot");
	//	Safe_Release(pInstance);
	//	return nullptr;
	//}

	return new CStrifeState_Shoot(pDevice, pContext, pOwner, pAnimOwner);
}

void CStrifeState_Shoot::Free()
{
	__super::Free();
}
