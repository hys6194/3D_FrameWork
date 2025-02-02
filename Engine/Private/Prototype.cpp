#include "Prototype.h"

#include "GameObject.h"
#include "Component.h"

Prototype_Manager::Prototype_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT Prototype_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pPrototype = new PROTOTYPES[iNumLevels];

	return S_OK;
}

HRESULT Prototype_Manager::Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, Base* pPrototype)
{
	//m_iNumLevels = iNumLevels;

	//if (nullptr == pPrototype)
	//	return E_FAIL;

	//m_pPrototype[m_iNumLevels].emplace(strPrototypeTag, pPrototype);

	if(iLevelIndex >= m_iNumLevels ||
		nullptr != Find_Prototype(iLevelIndex, strPrototypeTag))
		return E_FAIL;

	m_pPrototype[iLevelIndex].emplace(strPrototypeTag, pPrototype);

	return S_OK;
}

Base* Prototype_Manager::Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	Base* pPrototype = Find_Prototype(iLevelIndex, strPrototypeTag);
	if(nullptr == pPrototype)
		return nullptr;

	Base* pObject = { nullptr };

	if (PROTOTYPE::TYPE_GAMEOBJECT == ePrototype)
		pObject = dynamic_cast<GameObject*>(pPrototype)->Clone(pArg);
	else
		pObject = dynamic_cast<Component*>(pPrototype)->Clone(pArg);

	return pObject;

}

void Prototype_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pPrototype[iLevelIndex])
		Safe_Release(Pair.second);

	m_pPrototype[iLevelIndex].clear();
}

Base* Prototype_Manager::Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag)
{
	auto iter = m_pPrototype[iLevelIndex].find(strPrototypeTag);
	
	if (iter == m_pPrototype[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

Prototype_Manager* Prototype_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevels)
{
	Prototype_Manager* pInstance = new Prototype_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : Prototype_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Prototype_Manager::Free()
{
	__super::Free();

	for (_uint i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pPrototype[i])
			Safe_Release(Pair.second);

		m_pPrototype[i].clear();
	}

	Safe_Delete_Array(m_pPrototype);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
