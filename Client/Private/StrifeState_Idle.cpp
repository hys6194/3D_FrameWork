#include "StrifeState_Idle.h"
#include "Body_Player.h"
#include "Model.h"

StrifeState_Idle::StrifeState_Idle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
    : State{ pDevice , pContext, pOwner }
{
}

HRESULT StrifeState_Idle::Enter_State()
{
    m_pModelCom = dynamic_cast<Body_Player*>(m_pOwner)->Get_Model();

    m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::IDLE, true);

    m_pModelCom->Set_Interpolate(true);

    return S_OK;
}

void StrifeState_Idle::Update_State(_float fTimeDelta)
{   
    if (0 != m_pModelCom->Get_PreAnimIndex()
        && m_pModelCom->Get_Interpolate())
        m_pModelCom->Interpolate_Animation(0.2f);
    else
        m_pModelCom->Play_Animation(fTimeDelta);
}

HRESULT StrifeState_Idle::Exit_State()
{
    m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::IDLE);

    return S_OK;
}

StrifeState_Idle* StrifeState_Idle::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
{
    StrifeState_Idle* pInstance = new StrifeState_Idle(pDevice, pContext, pOwner);

    if (nullptr == pOwner)
    {
        MSG_BOX("Failed To Created : StrifeState_Idle");
        Safe_Release(pInstance);
        return nullptr;
    }

    //pInstance->Set_Owner(pOwner);
    //Safe_AddRef(m_pOwner);

    return pInstance;
}

void StrifeState_Idle::Free()
{
    __super::Free();
}
