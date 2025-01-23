#include "Prototype_Manager.h"

Prototype_Manager::Prototype_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

Prototype_Manager::~Prototype_Manager()
{
}

HRESULT Prototype_Manager::Initialize()
{
	return E_NOTIMPL;
}

Prototype_Manager* Prototype_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Prototype_Manager* pInstance = new Prototype_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed To Created : Level_GamePlay");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Prototype_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}

