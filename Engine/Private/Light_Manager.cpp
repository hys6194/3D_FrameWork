#include "Light_Manager.h"
#include "Light.h"

CLight_Manager::CLight_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice }
    , m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CLight_Manager::Initialize()
{
    return S_OK;
}

HRESULT CLight_Manager::Add_Light(const LIGHT_DESC& pDesc)
{
    CLight* pLight = CLight::Create(m_pDevice, m_pContext, pDesc);

    if (nullptr == pLight)
        return E_FAIL;

    m_listLights.push_back(pLight);

    return S_OK;
}

HRESULT CLight_Manager::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
    for (auto& pLight : m_listLights)
    {
        FAILED_CHECK_RETURN(pLight->Render(pShader, pVIBuffer), E_FAIL);
    }

    return S_OK;
}

CLight_Manager* CLight_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CLight_Manager* pInstance = new CLight_Manager(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed To Created : Light_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLight_Manager::Free()
{
    __super::Free();

    for (auto& iter : m_listLights)
        Safe_Release(iter);
    m_listLights.clear();

    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
