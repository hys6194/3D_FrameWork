#pragma once

#include "Attack_Base.h"
#include "Client_Defines.h"

BEGIN(Client)

class CGhoulAttack_Leaf : public CAttack_Base
{
private:
    CGhoulAttack_Leaf(CGameObject* pOwner, CGameObject* pAnimOwner);
    virtual ~CGhoulAttack_Leaf() = default;

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
    static CGhoulAttack_Leaf* Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex, _float fCoolTime);
    virtual void Free() override;
};

END