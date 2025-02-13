#include "Light.h"

Light::Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice }
    , m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT Light::Initialize(const LIGHT_DESC& pDesc)
{
    m_LightDesc = pDesc;

    return S_OK;
}

Light* Light::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHT_DESC& pDesc)
{
    Light* pInstance = new Light(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc)))
    {
        MSG_BOX("Failed To Created : Light");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Light::Free()
{
    __super::Free();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
