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

    // 이전에 진행하던 애니메이션의 인덱스를 Set하면서 정보를 알게 함

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
