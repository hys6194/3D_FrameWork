#include "FSM.h"
#include "State.h"

FSM::FSM(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component{ pDevice ,pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT FSM::Initialize_Prototype()
{
    return S_OK;
}

HRESULT FSM::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT FSM::Add_State(const _wstring& strStateTag, class State* pState)
{
    //if (FAILED(Find_State(strStateTag)))
    //    return E_FAIL;
    if (nullptr == pState)
        return E_FAIL;

    if(FAILED(Find_State(strStateTag)))
        m_mapFSM.emplace(strStateTag, pState);

    

    return S_OK;
}

HRESULT FSM::Find_State(const _wstring& strStateTag)
{
    //auto iter = find_if(m_mapFSM.begin(), m_mapFSM.end(),[&](State* pState)->_bool 
    //{
    //     if (0 != pState->Compare_StateName(strStateTag))
    //     {
    //         MSG_BOX("Failed_Find_State");
    //         return E_FAIL;
    //     }
    //});

    auto iter = m_mapFSM.find(strStateTag);

    if (iter == m_mapFSM.end())
        return E_FAIL;

    return S_OK;
}

HRESULT FSM::Delete_State()
{
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
    
    m_mapFSM.clear();

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
}
