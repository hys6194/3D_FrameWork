#include "StrifeState_Dash.h"
#include "Body_Player.h"
#include "Player.h"
#include "Model.h"	

StrifeState_Dash::StrifeState_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
	:State{pDevice, pContext, pOwner, pAnimOwner }
{
}

HRESULT StrifeState_Dash::Enter_State()
{
	Set_CurAnimation();

	return S_OK;
}

void StrifeState_Dash::PriorityUpdate_State(_float fTimeDelta)
{
}

void StrifeState_Dash::Update_State(_float fTimeDelta)
{
	Update_Animation(fTimeDelta);
}

void StrifeState_Dash::LateUpdate_State(_float fTimeDelta)
{
	if (false == m_pModelCom->Get_Interpolate())
		m_pModelCom->Reset_PreAnimation();
}

HRESULT StrifeState_Dash::Exit_State()
{
	Set_PreAnimation();

	return S_OK;
}

void StrifeState_Dash::Set_CurAnimation()
{
	m_pModelCom = dynamic_cast<Body_Player*>(m_pAnimOwner)->Get_Model();

	m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::DASH, true);

	m_pModelCom->Set_Interpolate(true);
}

void StrifeState_Dash::Update_Animation(_float fTimeDelta)
{
	if (0 != m_pModelCom->Get_PreAnimIndex()
		&& m_pModelCom->Get_Interpolate())
		m_pModelCom->Interpolate_Animation(0.2f);
	else
		m_pModelCom->Play_Animation(fTimeDelta);
}

void StrifeState_Dash::Set_PreAnimation()
{
	m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::RUN);
}

StrifeState_Dash* StrifeState_Dash::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
{
	StrifeState_Dash* pInstance = new StrifeState_Dash( pDevice, pContext, pOwner, pAnimOwner);

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
