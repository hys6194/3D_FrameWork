#include "StrifeState_Dash.h"
#include "Body_Player.h"
#include "Model.h"	

StrifeState_Dash::StrifeState_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
	:State{pDevice, pContext, pOwner }
{
}

HRESULT StrifeState_Dash::Enter_State()
{
	m_pModelCom = dynamic_cast<Body_Player*>(m_pOwner)->Get_Model();
	 
	m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH, false);

	return S_OK;
}

void StrifeState_Dash::Update_State(_float fTimeDelta)
{
	m_pModelCom->Play_Animation(fTimeDelta);
}

HRESULT StrifeState_Dash::Exit_State()
{
 
	return S_OK;
}

StrifeState_Dash* StrifeState_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
{
	StrifeState_Dash* pInstance = new StrifeState_Dash( pDevice, pContext, pOwner);

	if (nullptr == pOwner)
	{
		MSG_BOX("Failed To Created : BodyState_Idle");
		Safe_Release(pInstance);
		return nullptr;
	}

	return pInstance;
}

void StrifeState_Dash::Free()
{
	__super::Free();
}
