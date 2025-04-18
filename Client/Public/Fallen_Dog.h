#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)

class CFallen_Dog : public CMonster
{
private:
	CFallen_Dog(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CFallen_Dog(const CFallen_Dog& Prototype);
	virtual ~CFallen_Dog() = default;

public:
	enum FALLENDOG_ANIM {
		FALLENDOG_ATK_3HITCOMBO,
		FALLENDOG_ATK_BREATH,
		FALLENDOG_ATK_GROUNDSLAM,
		FALLENDOG_ATK_HEADBUTT,
		FALLENDOG_ATK_SLASH_L,
		FALLENDOG_ATK_SLASH_R,
		FALLENDOG_DEATH,
		FALLENDOG_IDLE,
		FALLENDOG_IMPACT_FRONT,
		FALLENDOG_RUN_F,
		FALLENDOG_SPAWN,
		FALLENDOG_TURN_90_L,
		FALLENDOG_TURN_90_R,
		FALLENDOG_WALK_B,
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
	static CFallen_Dog*						Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;


};

END