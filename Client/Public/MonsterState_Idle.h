#pragma once

#include "Client_Defines.h"
#include "MonsterState_Base.h"

BEGIN(Engine)
class CState;
class CModel;
END

BEGIN(Client)

class CMonsterState_Idle : public CMonsterState_Base
{
private:
	CMonsterState_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner);
	virtual ~CMonsterState_Idle() = default;


public:
	HRESULT							Enter_State() override;
	void							PriorityUpdate_State(_float fTimeDelta) override;
	void							Update_State(_float fTimeDelta) override;
	void							LateUpdate_State(_float fTimeDelta) override;
	HRESULT							Exit_State() override;

public:
	void							Set_PreAnimation() override;
	void							Update_Animation(_float fTimeDelta) override;
	void							Set_CurAnimation() override;

private:
	_float							m_fDistance = { 15.f };

	class CGameObject*				m_pPlayer = { nullptr };

public:
	static CMonsterState_Idle*		Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner, _uint AnimIndex);
	virtual void					Free() override;
};

END