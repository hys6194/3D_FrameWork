#include "Player_KeyState.h"

CPlayer_KeyState::CPlayer_KeyState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner, CGameInstance* pGameInstance)
	: CState{ pDevice , pContext, pOwner, pAnimOwner, pGameInstance }
{
}

HRESULT CPlayer_KeyState::Enter_State()
{


	return S_OK;
}

void CPlayer_KeyState::PriorityUpdate_State(_float fTimeDelta)
{

}

void CPlayer_KeyState::Update_State(_float fTimeDelta)
{
}

void CPlayer_KeyState::LateUpdate_State(_float fTimeDelta)
{
}
