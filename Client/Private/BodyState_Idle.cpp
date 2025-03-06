#include "BodyState_Idle.h"
#include "Body_Player.h"
#include "State.h"
#include "Model.h"

BodyState_Idle::BodyState_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
    : State{ pDevice , pContext, pOwner }
{
}

HRESULT BodyState_Idle::Enter_State()
{
    m_pBodyModelCom = dynamic_cast<Body_Player*>(m_pOwner)->Get_Model();

    m_pBodyModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::IDLE, true);

    return S_OK;
}

void BodyState_Idle::Update_State(_float fTimeDelta)
{
    m_pBodyModelCom->Play_Animation(fTimeDelta);
}

HRESULT BodyState_Idle::Exit_State()
{
    m_pBodyModelCom->Set_PreAnimationIndex(PLAYER_ANIMLIST::IDLE);

    return S_OK;
}

BodyState_Idle* BodyState_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
{
    BodyState_Idle* pInstance = new BodyState_Idle(pDevice, pContext, pOwner);

    if (nullptr == pOwner)
    {
        MSG_BOX("Failed To Created : BodyState_Idle");
        Safe_Release(pInstance);
        return nullptr;
    }

    //pInstance->Set_Owner(pOwner);
    //Safe_AddRef(m_pOwner);

    return pInstance;
}

void BodyState_Idle::Free()
{
    __super::Free();
}
