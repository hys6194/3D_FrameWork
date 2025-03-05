#include "FSM.h"
#include "State.h"

FSM::FSM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component{ pDevice ,pContext }
{
}

FSM::FSM(const FSM& Prototype)
    :Component{ Prototype }
{
}

HRESULT FSM::Initialize_Prototype()
{
    return S_OK;
}

HRESULT FSM::Initialize(void* pArg)
{
    // 여기서 뭘 해야할까
    // 일단 만들어 놓고 나중에 쓸모가없어지만 바로 new 리턴하는 걸로

    return S_OK;
}

HRESULT FSM::Add_State(const _uint& iState, class State* pState)
{
    if (nullptr == pState)
        return E_FAIL;
   
    if(nullptr == Find_State(iState))
        m_mapFSM.insert({ iState, pState });

    return S_OK;
}

void FSM::Update_State(_float fTimeDelta)
{
    m_pCurrentState->Update_State(fTimeDelta);
}

State* FSM::Find_State(const _uint& iState)
{
    auto iter = m_mapFSM.find(iState);

    if (iter == m_mapFSM.end())
        return nullptr;

    return iter->second;
}

HRESULT FSM::Delete_State()
{
    return S_OK;
}

HRESULT FSM::Change_State(const _uint& iState)
{
    auto iter = Find_State(iState);
    if (nullptr == iter || 
        m_pCurrentState == iter)
        return E_FAIL;

    if(0 != m_iCurrentState)
        m_pCurrentState->Exit_State();

    // 여기에서 State에서 Play한 애니메이션의 마지막 키프레임에 해당하는 뼈와
    // 바꾸려고 하는 애님의 첫번째 뼈를 가져와야 함 
    // Exit에서 마지막 키프레임에 해당하는 뼈를 순회하여 찾아야함
    // Enter 하면서 뼈를 가져오고 m_pOwner->ModelCom에게 뼈를 던져준다
    //m_pCurrentState->Get_Owner()->

    m_pCurrentState = iter;
    m_iCurrentState = iState;

    m_pCurrentState->Enter_State();

    // 여기에 뼈 던지기

    return S_OK;
}

FSM* FSM::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    FSM* pInstance = new FSM(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Cloned : FSM");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Component* FSM::Clone(void* pArg)
{
    Component* pInstance = new FSM(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : FSM");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void FSM::Free()
{
    __super::Free();

    for (auto& Pair : m_mapFSM)
        Safe_Release(Pair.second);
    
    m_mapFSM.clear();

    m_pCurrentState = nullptr;
}
