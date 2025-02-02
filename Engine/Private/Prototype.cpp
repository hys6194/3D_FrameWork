#include "Prototype.h"

Prototype::Prototype(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT Prototype::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pPrototype = new PROTOTYPES[iNumLevels];

	return S_OK;
}

Prototype* Prototype::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevels)
{
	Prototype* pInstance = new Prototype(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Prototype::Free()
{
	__super::Free();

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pPrototype[i])
			Safe_Release(Pair.second);

		m_pPrototype[i].clear();
	}

	Safe_Delete_Array(m_pPrototype);

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
