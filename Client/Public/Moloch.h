#pragma once

#include "Client_Defines.h"
#include "Monster.h"

BEGIN(Client)

class CMoloch : public CMonster
{
private:
	CMoloch(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CMoloch(const CMoloch& Prototype);
	virtual ~CMoloch() = default;

public:
    enum MOLOCH_ANIM {
        MOLOCH_TURN_90_L = 0,
        MOLOCH_TURN_90_R,
        MOLOCH_ATK_180_L,
        MOLOCH_ATK_180_R,
        MOLOCH_ATK_CRYSTALERUPTION,
        MOLOCH_ATK_CRYSTALERUPTION_02,
        MOLOCH_ATK_FULL_180_R,
        MOLOCH_ATK_FULL_GEYSER,
        MOLOCH_ATK_FULL_GEYSER_02,
        MOLOCH_ATK_FULL_GEYSER_START,
        MOLOCH_ATK_FULL_SWING_01,
        MOLOCH_ATK_FULL_SWING_02,
        MOLOCH_ATK_FULL_SWING_03,
        MOLOCH_ATK_SWIP,
        MOLOCH_ATK_SWIPE_02,
        MOLOCH_ATK_SWIPE_02_001,
        MOLOCH_ATK_SWIPE_03,
        MOLOCH_BLADE_EXTEND,
        MOLOCH_FULL_IDL,
        MOLOCH_FULL_IMPACT,
        MOLOCH_FULL_IMPACT_STUN,
        MOLOCH_IDLE,
        MOLOCH_INTRO,
        MOLOCH_RUN_F,
        MOLOCH_RUN_F_001
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
	static CMoloch*							Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CGameObject*					Clone(void* pArg);
	virtual void							Free() override;


};

END