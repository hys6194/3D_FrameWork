#include "StrifeState_Shoot.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"	

#include "GameInstance.h"

StrifeState_Shoot::StrifeState_Shoot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
	:State{pDevice, pContext, pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT StrifeState_Shoot::Enter_State()
{
	Set_CurAnimation();

	return S_OK;
}

void StrifeState_Shoot::PriorityUpdate_State(_float fTimeDelta)
{
	m_iKeyState = dynamic_cast<Player*>(m_pOwner)->Get_PlayerKeyState();

	// ÃÑÀ» ½ð´Ù
	if (m_iKeyState & Player::KEY_LB)
	{

	}

	// ¾È ½ð´Ù
	else
	{
			
	}

}

void StrifeState_Shoot::Update_State(_float fTimeDelta)
{
	Update_Animation(fTimeDelta);
}

void StrifeState_Shoot::LateUpdate_State(_float fTimeDelta)
{
	if (false == m_pModelCom->Get_Interpolate())
		m_pModelCom->Reset_PreAnimation();
}

HRESULT StrifeState_Shoot::Exit_State()
{
	Set_PreAnimation();

	return S_OK;
}

void StrifeState_Shoot::Set_PreAnimation()
{
	m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::AIM_IDLE);
}

void StrifeState_Shoot::Update_Animation(_float fTimeDelta)
{
	//if (0 != m_pModelCom->Get_PreAnimIndex()
	//	&& m_pModelCom->Get_Interpolate())
	//	m_pModelCom->Interpolate_Animation(0.2f);
	//else
		m_pModelCom->Play_Animation(fTimeDelta);
}

void StrifeState_Shoot::Set_CurAnimation()
{
	m_pModelCom = dynamic_cast<Body_Player*>(m_pAnimOwner)->Get_Model();
	
	m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::AIM_IDLE, true);
}

StrifeState_Shoot* StrifeState_Shoot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
{
	StrifeState_Shoot* pInstance = new StrifeState_Shoot(pDevice, pContext, pOwner, pAnimOwner);

	if (nullptr == pOwner)
	{
		MSG_BOX("Failed To Created : StrifeState_Shoot");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

//StrifeState_Shoot* StrifeState_Shoot::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iState, GameObject* pOwner)
//{
//	StrifeState_Shoot* pInstance = new StrifeState_Shoot( pDevice, pContext, pOwner);
//
//	if (nullptr == pOwner)
//	{
//		MSG_BOX("Failed To Created : BodyState_Idle");
//		Safe_Release(pInstance);
//		return nullptr;
//	}
//
//	return pInstance;
//}

void StrifeState_Shoot::Free()
{
	__super::Free();
}
