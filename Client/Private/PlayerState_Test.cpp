#include "PlayerState_Test.h"

PlayerState_Test::PlayerState_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

HRESULT PlayerState_Test::Enter_State()
{
	return S_OK;
}

void PlayerState_Test::Update_State()
{
}

HRESULT PlayerState_Test::Exit_State()
{
	return S_OK;
}

PlayerState_Test* PlayerState_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iState)
{
	PlayerState_Test* pInstance = new PlayerState_Test( pDevice, pContext );

	//if (FAILED(pInstance->Enter_State()))
	//{
	//	MSG_BOX("Failed To Created : PlayerState_Test");
	//	Safe_Release(pInstance);
	//}

	return pInstance;
}

void PlayerState_Test::Free()
{
	__super::Free();
}
