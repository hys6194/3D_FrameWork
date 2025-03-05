#include "BodyState_Run.h"
#include "Body_Player.h"
#include "Model.h"

BodyState_Run::BodyState_Run(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
    : State{ pDevice , pContext, pOwner }
{
}

HRESULT BodyState_Run::Enter_State()
{
    m_pBodyModelCom = dynamic_cast<Body_Player*>(m_pOwner)->Get_Model();

    m_pBodyModelCom->Set_AnimationIndex(22, true);

    return S_OK;
}

void BodyState_Run::Update_State(_float fTimeDelta)
{
    m_pBodyModelCom->Play_Animation(fTimeDelta);
}

HRESULT BodyState_Run::Exit_State()
{
    return S_OK;
}

BodyState_Run* BodyState_Run::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
{
    BodyState_Run* pInstance = new BodyState_Run(pDevice, pContext, pOwner);

    if (nullptr == pOwner)
    {
        MSG_BOX("Failed To Created : BodyState_Run");
        Safe_Release(pInstance);
        return nullptr;
    }

    return pInstance;
}

void BodyState_Run::Free()
{
    __super::Free();
}
