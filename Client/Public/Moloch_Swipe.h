#pragma once

#include "Attack_Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CMoloch_Swipe : public CAttack_Base
{
private:
	CMoloch_Swipe(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CMoloch_Swipe() = default;

public:
	virtual HRESULT							Enter_State();
	virtual void							PriorityUpdate_State(_float fTimeDelta);
	virtual void							Update_State(_float fTimeDelta);
	virtual void							LateUpdate_State(_float fTimeDelta);
	virtual HRESULT							Exit_State();

	virtual void							Set_PreAnimation();
	virtual void							Update_Animation(_float fTimeDelta);
	virtual void							Set_CurAnimation();

	virtual void							Update_CoolTime(_float fTimeDelta);
	virtual _bool							Check_Attackable();

public:

public:
	static CMoloch_Swipe*					Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime);
	virtual void							Free();


};

END