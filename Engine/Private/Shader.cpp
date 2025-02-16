#include "Shader.h"

Shader::Shader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :Component{ pDevice , pContext }
{
}

Shader::Shader(const Shader& Prototype)
    : Component{ Prototype }
{
}

HRESULT Shader::Initialize_Prototype(const _tchar* pShaderFilePath)
{
    return S_OK;
}

HRESULT Shader::Initialize(void* pArg)
{
    return S_OK;
}

Shader* Shader::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath)
{
    Shader* pInstance = new Shader(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pShaderFilePath)))
    {
        MSG_BOX("Failed To Created : Shader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Component* Shader::Clone(void* pArg)
{
    Shader* pInstance = new Shader(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Shader");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Shader::Free()
{
    __super::Free();
}
