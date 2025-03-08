#include "PlayerState_Test.h"
#include "Model.h"	

PlayerState_Test::PlayerState_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
	:State{pDevice, pContext, pOwner }
{
}

HRESULT PlayerState_Test::Enter_State()
{
	//m_pModelCom->Set_AnimationIndex();

	//m_pModelCom = dynamic_cast<*>(m_pOwner)->Get_Model();
	// 
	//m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::RUN, true);

	return S_OK;
}

void PlayerState_Test::Update_State(_float fTimeDelta)
{
}

HRESULT PlayerState_Test::Exit_State()
{
	return S_OK;
}

PlayerState_Test* PlayerState_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
{
	PlayerState_Test* pInstance = new PlayerState_Test(pDevice, pContext, pOwner);

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
