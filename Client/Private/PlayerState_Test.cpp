#include "PlayerState_Test.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"	

#include "GameInstance.h"

PlayerState_Test::PlayerState_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
	:State{pDevice, pContext, pOwner, pAnimOwner, m_pGameInstance }
{
}

HRESULT PlayerState_Test::Enter_State()
{
	Set_CurAnimation();

	return S_OK;
}

void PlayerState_Test::PriorityUpdate_State(_float fTimeDelta)
{
	m_iKeyState = dynamic_cast<Player*>(m_pOwner)->Get_PlayerState();
}

void PlayerState_Test::Update_State(_float fTimeDelta)
{
	Update_Animation(fTimeDelta);
}

void PlayerState_Test::LateUpdate_State(_float fTimeDelta)
{
	if (false == m_pModelCom->Get_Interpolate())
		m_pModelCom->Reset_PreAnimation();
}

HRESULT PlayerState_Test::Exit_State()
{
	Set_PreAnimation();

	return S_OK;
}

void PlayerState_Test::Set_PreAnimation()
{
	m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::RUN);
}

void PlayerState_Test::Update_Animation(_float fTimeDelta)
{
	if (0 != m_pModelCom->Get_PreAnimIndex()
		&& m_pModelCom->Get_Interpolate())
		m_pModelCom->Interpolate_Animation(0.2f);
	else
		m_pModelCom->Play_Animation(fTimeDelta);
}

void PlayerState_Test::Set_CurAnimation()
{
	m_pModelCom = dynamic_cast<Body_Player*>(m_pAnimOwner)->Get_Model();
	
	m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::RUN, true);
}

PlayerState_Test* PlayerState_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
{
	PlayerState_Test* pInstance = new PlayerState_Test(pDevice, pContext, pOwner, pAnimOwner);

	if (nullptr == pOwner)
	{
		MSG_BOX("Failed To Created : BodyState_Idle");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

//PlayerState_Test* PlayerState_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iState, GameObject* pOwner)
//{
//	PlayerState_Test* pInstance = new PlayerState_Test( pDevice, pContext, pOwner);
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

void PlayerState_Test::Free()
{
	__super::Free();
}
