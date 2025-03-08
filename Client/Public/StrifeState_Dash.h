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
	StrifeState_Dash(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner);
	virtual ~StrifeState_Dash() = default;

public:
	virtual HRESULT Enter_State();
	virtual void Update_State(_float fTimeDelta);
	virtual HRESULT Exit_State();

private:
	Model*			m_pModelCom;

public:
	static StrifeState_Dash* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner);
	virtual void Free();
};

END