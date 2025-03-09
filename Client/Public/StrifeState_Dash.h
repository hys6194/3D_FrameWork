#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class Model;
END

BEGIN(Client)

class StrifeState_Dash : public State
{
private	:
	StrifeState_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner);
	virtual ~StrifeState_Dash() = default;

public:
	virtual HRESULT Enter_State();
	virtual void PriorityUpdate_State(_float fTimeDelta);
	virtual void Update_State(_float fTimeDelta);
	virtual void LateUpdate_State(_float fTimeDelta);
	virtual HRESULT Exit_State();

public:
	virtual void Set_CurAnimation();
	virtual void Update_Animation(_float fTimeDelta);
	virtual void Set_PreAnimation();

private:
	Model*				m_pModelCom = { nullptr };
	_bool				m_bDashed = { false };
	_uint				m_iKeyState = { 0 };


public:
	static StrifeState_Dash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner);
	virtual void Free();
};

END