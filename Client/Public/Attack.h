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
	CAttack_Base*						Find_AttackPattern(_uint iStateTag);

	// 어떻게 해야 뒤 돌았을 때의 패턴을 찾고 이를 랜덤으로 뽑아낼까
	// 차라리 하드 코딩을 해야할 수도 있는데 그렇게 처리할까 
	// 딱히 아이디어가 떠오르지 않는다
	CAttack_Base*						Find_Attackable(_bool bSoruColl, _bool bDestColl);
public:
	void								Regist_AttackPattern(_uint iStateEnum, CAttack_Base* pState);

private:
	map<const _uint ,CAttack_Base*>		m_mapPattern;

	vector<CAttack_Base*>				m_vecPattern;

	CAttack_Base*						m_pAttackState;

	_float								m_fTotalTime = {};

public:
	static CAttack*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent*					Clone(void* pArg);
	virtual void						Free() override;
};


END