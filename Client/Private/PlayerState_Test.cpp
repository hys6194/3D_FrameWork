#include "PlayerState_Test.h"
#include "State.h"


PlayerState_Test::PlayerState_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:State{pDevice, pContext}
{
}

HRESULT PlayerState_Test::Enter_State()
{
	

	return S_OK;
}

void PlayerState_Test::Update_State(_float fTimeDelta)
{
}

HRESULT PlayerState_Test::Exit_State()
{
	return S_OK;
}

PlayerState_Test* PlayerState_Test::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iState, GameObject* pOwner)
{
	PlayerState_Test* pInstance = new PlayerState_Test( pDevice, pContext );

	if (nullptr == pOwner)
		return nullptr;

	pInstance->Set_Owner(pOwner);

	return pInstance;
}

void PlayerState_Test::Free()
{
	__super::Free();
}
