#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class Model;
END

BEGIN(Client)

class StrifeState_Shoot : public State
{
private	:
	StrifeState_Shoot(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner);
	virtual ~StrifeState_Shoot() = default;

public:
	virtual HRESULT			Enter_State();
	virtual void			PriorityUpdate_State(_float fTimeDelta);
	virtual void			Update_State(_float fTimeDelta);
	virtual void			LateUpdate_State(_float fTimeDelta);
	virtual HRESULT			Exit_State();

public:
	virtual void			Set_CurAnimation();
	virtual void			Update_Animation(_float fTimeDelta);
	virtual void			Set_PreAnimation();


private:
	Model*					m_pModelCom;
	_uint					m_iKeyState = { 0 };

public:
	static StrifeState_Shoot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner);
	virtual void Free();

	//static StrifeState_Shoot* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iState, GameObject* pOwner);
};

END