#pragma once

#include "Client_Defines.h"
#include "Component.h"
#include "Attack_Base.h"

BEGIN(Engine)
class CComponent;
END

BEGIN(Client)

class CAttack final : public CComponent
{
private: 
	CAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CAttack(const CAttack& Prototype);
	virtual ~CAttack() = default;

public:
	virtual HRESULT						Initialize_Prototype() override;
	virtual HRESULT						Initialize(void* pArg) override;

public:
	void								Update_CoolTime(_float fTimeDelta);
	
	CAttack_Base*						Find_Attackable();

	//HRESULT							Enter_State();
	//void								PriorityUpdate_State(_float fTimeDelta);
	//void								Update_State(_float fTimeDelta);
	//void								LateUpdate_State(_float fTimeDelta);
	//HRESULT							Exit_State();

public:
	// 등록은 해야하는건 맞으나 해제는 해야할까?

	void								Regist_AttackPattern(_uint iStateEnum, CAttack_Base* pState);
	void								Secede_AttackPattern();


private:
	map<const _uint ,CAttack_Base*>		m_mapPattern;

	CAttack_Base*						m_pAttackState;

	_float								m_fTotalTime = {};

public:
	static CAttack*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*					Clone(void* pArg);
	virtual void						Free() override;
};


END