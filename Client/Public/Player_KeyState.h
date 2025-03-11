#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class State;
END

BEGIN(Client)

class CPlayer_KeyState : public State
{
private:
	CPlayer_KeyState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner, CGameInstance* pGameInstance);
	virtual ~CPlayer_KeyState() = default;

public:
	virtual HRESULT Enter_State() override;
	virtual void	PriorityUpdate_State(_float fTimeDelta) override;
	virtual void	Update_State(_float fTimeDelta) override;
	virtual void	LateUpdate_State(_float fTimeDelta) override;
	virtual HRESULT Exit_State() = 0;

protected:
	_uint 			m_iKeyState = {};		

};

END