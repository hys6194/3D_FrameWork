#pragma once

#include "Attack_Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CGhoulAttack_Flurry : public CAttack_Base
{
private:
    CGhoulAttack_Flurry(CGameObject* pOwner, CGameObject* pAnimOwner);
    virtual ~CGhoulAttack_Flurry() = default;

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
    virtual _bool                               Check_Condition() override;

public:
    static CGhoulAttack_Flurry* Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime);
    virtual void Free() override;
};

END