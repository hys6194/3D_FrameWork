#include "Layer.h"

#include "GameObject.h"

CLayer::CLayer()
{
}

HRESULT CLayer::Add_GameObject(CGameObject* pGameObject)
{
	m_GameObjects.push_back(pGameObject);

    return S_OK;
}

void CLayer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Priority_Update(fTimeDelta);
}

void CLayer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Update(fTimeDelta);
}

void CLayer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Late_Update(fTimeDelta);
}

CGameObject* CLayer::Find_Object(const _tchar* strObjectTag)
{
	for (auto& iter : m_GameObjects)
	{
		if(true == iter->Compare_Name(strObjectTag))
			return iter;
	}

	return nullptr;
}

CLayer* CLayer::Create()
{
	return new CLayer;
}

void CLayer::Free()
{

 	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
