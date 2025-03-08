#include "StrifeState_Run.h"
#include "Body_Player.h"
#include "Model.h"

StrifeState_Run::StrifeState_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
    : State{ pDevice , pContext, pOwner, pAnimOwner }
{
}

HRESULT StrifeState_Run::Enter_State()
{
    Set_CurAnimation();


    return S_OK;
}

void StrifeState_Run::PriorityUpdate_State(_float fTimeDelta)
{

}

void StrifeState_Run::Update_State(_float fTimeDelta)
{   
    Update_Animation(fTimeDelta);

    m_pOwner->Get_Transform()->Go_Straight(fTimeDelta);

}

void StrifeState_Run::LateUpdate_State(_float fTimeDelta)
{
    if (false == m_pModelCom->Get_Interpolate())
        m_pModelCom->Reset_PreAnimation();
}

HRESULT StrifeState_Run::Exit_State()
{
    Set_PreAnimation();
    return S_OK;
}

void StrifeState_Run::Set_CurAnimation()
{
    m_pModelCom = dynamic_cast<Body_Player*>(m_pAnimOwner)->Get_Model();

    m_pModelCom->Set_AnimationIndex(PLAYER_ANIMLIST::RUN, true);

    m_pModelCom->Set_Interpolate(true);


}

void StrifeState_Run::Update_Animation(_float fTimeDelta)
{
    if (0 != m_pModelCom->Get_PreAnimIndex()
        && m_pModelCom->Get_Interpolate())
        m_pModelCom->Interpolate_Animation(0.2f);
    else
        m_pModelCom->Play_Animation(fTimeDelta);
}

void StrifeState_Run::Set_PreAnimation()
{
    m_pModelCom->Set_PreAnimation(PLAYER_ANIMLIST::RUN);
}

StrifeState_Run* StrifeState_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner, GameObject* pAnimOwner)
{
    StrifeState_Run* pInstance = new StrifeState_Run(pDevice, pContext, pOwner, pAnimOwner);

    if (nullptr == pOwner)
    {
        MSG_BOX("Failed To Created : StrifeState_Run");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void StrifeState_Run::Free()
{
    __super::Free();
}
