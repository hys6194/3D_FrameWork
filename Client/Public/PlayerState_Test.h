#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class State;
END

BEGIN(Client)

class PlayerState_Test : public State
{
private	:
	PlayerState_Test(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~PlayerState_Test() = default;

public:
	virtual HRESULT Enter_State();
	virtual void Update_State();
	virtual HRESULT Exit_State();

public:
	static PlayerState_Test* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iState);
	virtual void Free();
};

END