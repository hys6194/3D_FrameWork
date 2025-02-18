#include "Model.h"

Model::Model(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :Component{ pDevice , pContext }
{
}

Model::Model(const Model& Prototype)
    : Component{ Prototype }
{
}

HRESULT Model::Initialize_Prototype(const _char* pFilePath)
{

    return S_OK;
}

HRESULT Model::Initialize(void* pArg)
{
    return S_OK;
}

Model* Model::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _char* pFilePath)
{
    Model* pInstance = new Model(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pFilePath)))
    {
        MSG_BOX("Failed To Created : Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Component* Model::Clone(void* pArg)
{
    Component* pInstance = new Model(m_pDevice, m_pContext);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Model");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Model::Free()
{
    __super::Free();
}
