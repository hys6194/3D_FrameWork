#include "Light.h"
#include "Shader.h"
#include "VIBuffer_Rect.h"


CLight::CLight(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{

}

HRESULT CLight::Initialize(const LIGHT_DESC& pDesc)
{
    m_LightDesc = pDesc;

    return S_OK;
}

HRESULT CLight::Render(CShader* pShader, CVIBuffer_Rect* pVIBuffer)
{
    _uint       iPassIndex = { 0 };

    if (LIGHT_DESC::TYPE_DIRECTIONAL == m_LightDesc.eType)
    {
        iPassIndex = 1;

        if (FAILED(pShader->Bind_RawValue("g_vLightDir", &m_LightDesc.vDirection, sizeof(_float4))))
            return E_FAIL;
    }
    else
    {
        iPassIndex = 2;
    }

    pShader->Begin(iPassIndex);

    pVIBuffer->Render();

    return S_OK;
}

CLight* CLight::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHT_DESC& pDesc)
{
    CLight* pInstance = new CLight(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pDesc)))
    {
        MSG_BOX("Failed To Created : Light");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CLight::Free()
{
    __super::Free();

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
}
