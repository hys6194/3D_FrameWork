#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)

class CGhoul : public CMonster
{
private:
	CGhoul(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CGhoul(const CGhoul& Prototype);
	virtual ~CGhoul() = default;

public:
	enum GHOUL_ANIMLIST
	{
		GHOUL_DEAD_POSE = 0,
		GHOUL_DEATH,
		GHOUL_EAT, 
		GHOUL_EVADE_LEFT,
		GHOUL_EVADE_RIGHT,
		GHOUL_EXCUTION,
		GHOUL_IDLE,
		GHOUL_IMPACT_F,
		GHOUL_IMPACT_L,
		GHOUL_SPAWN,
		GHOUL_TAUNT,
		GHOUL_RUN,
		GHOUL_TURN_LEFT,
		GHOUL_TURN_RIGHT,
		GHOUL_WALK_B,
		GHOUL_WALK_F,
		GHOUL_WALK_L,
	};

public:
	virtual HRESULT							Initialize_Prototype() override;
	virtual HRESULT							Initialize(void* pArg) override;
	virtual void							Priority_Update(_float fTimeDelta) override;
	virtual void							Update(_float fTimeDelta) override;
	virtual void							Late_Update(_float fTimeDelta) override;
	virtual HRESULT							Render() override;


public:
	virtual HRESULT							Ready_PartObjects();
	virtual HRESULT							Ready_Components();

public:
	HRESULT									Ready_States();
public:
	static CGhoul*							Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;


};

END