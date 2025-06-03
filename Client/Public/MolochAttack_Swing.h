#pragma once

#include "Attack_Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CMolochAttack_Swing : public CAttack_Base
{
private:
	CMolochAttack_Swing(CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CMolochAttack_Swing() = default;

public:
	virtual HRESULT							Enter_State();
	virtual void							PriorityUpdate_State(_float fTimeDelta);
	virtual void							Update_State(_float fTimeDelta);
	virtual void							LateUpdate_State(_float fTimeDelta);
	virtual HRESULT							Exit_State();

	virtual void							Set_PreAnimation();
	virtual void							Update_Animation(_float fTimeDelta) override;
	virtual void							Set_CurAnimation();

	virtual void							Update_CoolTime(_float fTimeDelta);
	virtual _bool							Check_Attackable();
	virtual _bool							Check_Colls();

private:
	_bool									    m_bSpawn = { false };


public:
	static CMolochAttack_Swing*					Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime);
	virtual void							Free() override;


};

END