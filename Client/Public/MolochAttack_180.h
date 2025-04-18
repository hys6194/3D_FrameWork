#pragma once

#include "Attack_Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CMolochAttack_180 : public CAttack_Base
{
private:
    CMolochAttack_180(CGameObject* pOwner, CGameObject* pAnimOwner);
    virtual ~CMolochAttack_180() = default;

public:
    virtual HRESULT                             Enter_State() override;
    virtual void                                PriorityUpdate_State(_float fTimeDelta) override;
    virtual void                                Update_State(_float fTimeDelta) override;
    virtual void                                LateUpdate_State(_float fTimeDelta) override;
    virtual HRESULT                             Exit_State() override;

public:
    virtual void                                Set_PreAnimation() override;
    virtual void                                Set_CurAnimation() override;
    virtual void                                Update_CoolTime(_float fTimeDelta) override;
    virtual void                                Update_Animation(_float fTimeDelta) override;

    virtual _bool                               Check_Attackable() override;
    virtual _bool                               Check_Colls() override;

private:
    _bool									    m_bSpawn = { false };
    _bool									    m_bTurned = { false };

    _vector                                     m_vTurnDir = {};

public:
    static CMolochAttack_180* Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime);
    virtual void Free() override;
};

END