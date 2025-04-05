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
    enum GHOUL_ANIMLIST {
        GHOUL_ATK_FLURRY,
        GHOUL_ATK_LEAP,
        GHOUL_DEADPOSE,
        GHOUL_DEATH,
        GHOUL_EAT,
        GHOUL_EVADE_LEFT,
        GHOUL_EVADE_RIGHT,
        GHOUL_IA_DEATH_STRIFE,
        GHOUL_IDLE,
        GHOUL_IMPACT_F,
        GHOUL_IMPACT_L,
        GHOUL_RUN_F,
        GHOUL_SPAWN_CLIMB,
        GHOUL_TAUNT_A,
        GHOUL_TURN90_L,
        GHOUL_TURN90_R,
        GHOUL_WALK_B,
        GHOUL_WALK_F,
        GHOUL_WALK_L,
        GHOUL_WALK_R
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