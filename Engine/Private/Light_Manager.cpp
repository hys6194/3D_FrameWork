#include "Light_Manager.h"

Light_Manager::Light_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice }
    , m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT Light_Manager::Initialize()
{
    return S_OK;
}

HRESULT Light_Manager::Add_Light(const LIGHT_DESC& pDesc)
{
    Light* pLight;

    
    m_listLights.push_back(pLight);

    return S_OK;
}

Light_Manager* Light_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Light_Manager* pInstance = new Light_Manager(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed To Created : Light_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Light_Manager::Free()
{
    __super::Free();


    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
