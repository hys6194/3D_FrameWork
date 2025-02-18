#include "Mesh.h"

Mesh::Mesh(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : VIBuffer{ pDevice, pContext }
{
}

Mesh::Mesh(const Mesh& Prtotype)
    : VIBuffer{ Prototype }
{
}

HRESULT Mesh::Initialize_Prototype(const aiMesh* pAIMesh)
{
    return E_NOTIMPL;
}

HRESULT Mesh::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

Mesh* Mesh::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const aiMesh* pAIMesh)
{
	Mesh* pInstance = new Mesh(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pAIMesh)))
	{
		MSG_BOX("Failed To Created : Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component* Mesh::Clone(void* pArg)
{
	Component* pInstance = new Mesh(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Mesh");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Mesh::Free()
{
    __super::Free();
}
