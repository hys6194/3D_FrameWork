#include "Prototype.h"

#include "GameObject.h"
#include "Component.h"

CPrototype_Manager::CPrototype_Manager(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext{ pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT CPrototype_Manager::Initialize(_uint iNumLevels)
{
	m_iNumLevels = iNumLevels;

	m_pPrototype = new PROTOTYPES[iNumLevels];

	return S_OK;
}

HRESULT CPrototype_Manager::Add_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag, CBase* pPrototype)
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

CBase* CPrototype_Manager::Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
 	if (iLevelIndex >= m_iNumLevels)
		return nullptr;

	CBase* pPrototype = Find_Prototype(iLevelIndex, strPrototypeTag);
	if(nullptr == pPrototype)
		return nullptr;

	CBase* pObject = { nullptr };

	if (PROTOTYPE::TYPE_GAMEOBJECT == ePrototype)
		pObject = dynamic_cast<CGameObject*>(pPrototype)->Clone(pArg);
	else
		pObject = dynamic_cast<CComponent*>(pPrototype)->Clone(pArg);

	return pObject;

}

void CPrototype_Manager::Clear(_uint iLevelIndex)
{
	if (iLevelIndex >= m_iNumLevels)
		return;

	for (auto& Pair : m_pPrototype[iLevelIndex])
		Safe_Release(Pair.second);

	m_pPrototype[iLevelIndex].clear();
}

CBase* CPrototype_Manager::Find_Prototype(_uint iLevelIndex, const wstring& strPrototypeTag)
{
	auto iter = m_pPrototype[iLevelIndex].find(strPrototypeTag);
	
	if (iter == m_pPrototype[iLevelIndex].end())
		return nullptr;

	return iter->second;
}

CPrototype_Manager* CPrototype_Manager::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, _uint iNumLevels)
{
	CPrototype_Manager* pInstance = new CPrototype_Manager(pDevice, pContext);

	if (FAILED(pInstance->Initialize(iNumLevels)))
	{
		MSG_BOX("Failed to Created : Prototype_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPrototype_Manager::Free()
{
	__super::Free();

	for (size_t i = 0; i < m_iNumLevels; ++i)
	{
		for (auto& Pair : m_pPrototype[i])
			Safe_Release(Pair.second);

		m_pPrototype[i].clear();
	}

	Safe_Delete_Array(m_pPrototype);

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
