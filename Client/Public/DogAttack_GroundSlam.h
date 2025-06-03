#pragma once

#include "Attack_Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CDogAttack_GroundSlam : public CAttack_Base
{
private:
    CDogAttack_GroundSlam(CGameObject* pOwner, CGameObject* pAnimOwner);
    virtual ~CDogAttack_GroundSlam() = default;

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

public:
    static CDogAttack_GroundSlam* Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime);
    virtual void Free() override;
};

END