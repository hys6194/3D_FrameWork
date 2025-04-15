#include "Moloch_Swipe.h"
#include "GameInstance.h"
#include "Player.h"

#include "Moloch.h"
//#include "Ghoul.h"

CMoloch_Swipe::CMoloch_Swipe(CGameObject* pOwner, CGameObject* pAnimOwner)
    : CAttack_Base { pOwner, pAnimOwner }
{
}

HRESULT CMoloch_Swipe::Enter_State()
{
    Setting_PlayerInfo();
    Set_CurAnimation();

    m_iAnimIndex = CMoloch::MOLOCH_ATK_SWIPE_01;

    return S_OK;
}

void CMoloch_Swipe::PriorityUpdate_State(_float fTimeDelta)
{
}

void CMoloch_Swipe::Update_State(_float fTimeDelta)
{
    Update_Animation(fTimeDelta);
}

void CMoloch_Swipe::LateUpdate_State(_float fTimeDelta)
{
}

HRESULT CMoloch_Swipe::Exit_State()
{
    return S_OK;
}

void CMoloch_Swipe::Set_PreAnimation()
{
}

void CMoloch_Swipe::Update_Animation(_float fTimeDelta)
{
    __super::Update_Animation(fTimeDelta);
}

void CMoloch_Swipe::Set_CurAnimation()
{
}

void CMoloch_Swipe::Update_CoolTime(_float fTimeDelta)
{
    if (m_bAttack /*|| m_fCoolTime이 원래 쿨타임과 같아지거나 커졌다면 */)
        return;

    m_fCoolTime += fTimeDelta;
}

CMoloch_Swipe* CMoloch_Swipe::Create(CGameObject* pOwner, CGameObject* pAnimOwner, _uint iAnimIndex)
{
    CMoloch_Swipe* pInstance = new CMoloch_Swipe(pOwner, pAnimOwner);

    pInstance->m_iAnimIndex = iAnimIndex;

    if (nullptr == pInstance)
        return nullptr;

    return pInstance;
}

void CMoloch_Swipe::Free()
{
}
