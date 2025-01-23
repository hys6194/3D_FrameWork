#include "Prototype.h"



Prototype::Prototype(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT Prototype::Initialize(_uint iNumLevel)
{
	m_iNumLevel = iNumLevel;

	m_pPrototype = new PROTOTYPES[m_iNumLevel];

	return S_OK;
}

HRESULT Prototype::Add_Prototype(const _wstring& szPrototypeName, _uint iLevelIndex, Base* pPrototype)
{
	if (iLevelIndex >= m_iNumLevel ||
		nullptr != Find_Prototype(szPrototypeName, iLevelIndex))
		return E_FAIL;

	m_pPrototype[iLevelIndex].emplace(szPrototypeName, pPrototype);

	return S_OK;
}

Base* Prototype::Clone_Prototype(PROTOTYPE ePrototype, const _wstring& szPrototypeName, _uint iLevelIndex, void* pArg)
{
	if (iLevelIndex >= m_iNumLevel)
		return nullptr;

	Base* pPrototype = Find_Prototype(szPrototypeName, iLevelIndex);
	if (nullptr == pPrototype)
		return nullptr;

	Base* pObject = { nullptr };

	if (PROTOTYPE::TYPE_GAMEOBJECT == ePrototype)
		//pObject = dynamic_cast<GameObject*>(pObject)->Clone(pArg);	

	if (PROTOTYPE::TYPE_COMPONENT == ePrototype)
		//pObject = dynamic_cast<Component*>(pObject)->Clone(pArg);


	return pObject;
}

void Prototype::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevel)
		return;

	for (auto& Pair : m_pPrototype[iLevelIndex])
		Safe_Release(Pair.second);

	m_pPrototype[iLevelIndex].clear();

}

Base* Prototype::Find_Prototype(const _wstring& szPrototypeName, _uint iLevelIndex)
{
	auto iter = m_pPrototype[iLevelIndex].find(szPrototypeName);

	if (iter == m_pPrototype[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

Prototype* Prototype::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevel)
{
	Prototype* pInstance = new Prototype(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iNumLevel)))
	{
		MSG_BOX("Failed to Created : Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Prototype::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevel; i++)
	{
		for (auto& Pair : m_pPrototype[i])
			Safe_Release(Pair.second);

		m_pPrototype[i].clear();
	}

	Safe_Delete_Array(m_pPrototype);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
