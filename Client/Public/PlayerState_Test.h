#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class Model;
END

BEGIN(Client)

class PlayerState_Test : public State
{
private	:
	PlayerState_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner);
	virtual ~PlayerState_Test() = default;

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
	Model*		m_pModelCom;

public:
	static PlayerState_Test* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner);
	virtual void Free();

	//static PlayerState_Test* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iState, GameObject* pOwner);
};

END