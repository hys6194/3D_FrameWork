#pragma once

#include "Client_Defines.h"
#include "State.h"

BEGIN(Engine)
class CModel;
END

BEGIN(Client)

class PlayerState_Test : public CState
{
private	:
	PlayerState_Test(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~PlayerState_Test() = default;

public:
	virtual HRESULT		Enter_State();
	virtual void		PriorityUpdate_State(_float fTimeDelta);
	virtual void		Update_State(_float fTimeDelta);
	virtual void		LateUpdate_State(_float fTimeDelta);
	virtual HRESULT		Exit_State();

public:
	virtual void		Set_CurAnimation();
	virtual void		Update_Animation(_float fTimeDelta);
	virtual void		Set_PreAnimation();


private:
	CModel*				m_pModelCom;
	_uint				m_iKeyState = { 0 };

public:
	static PlayerState_Test* Create(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual void Free();

};

END