#pragma once


// 이동 관련된 클래스 들은 이 클래스를 상속받아서 이동한다
#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class CState;
END

BEGIN(Client)

class CPlayer_KeyState : public CState
{
protected:
	CPlayer_KeyState(CGameObject* pOwner, CGameObject* pAnimOwner, CGameInstance* pGameInstance);
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