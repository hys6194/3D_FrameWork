#include "Layer.h"

#include "GameObject.h"

Layer::Layer()
{
}

HRESULT Layer::Add_GameObject(GameObject* pGameObject)
{
	m_GameObjects.push_back(pGameObject);

    return S_OK;
}

void Layer::Priority_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Priority_Update(fTimeDelta);
}

void Layer::Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Update(fTimeDelta);
}

void Layer::Late_Update(_float fTimeDelta)
{
	for (auto& pGameObject : m_GameObjects)
		pGameObject->Late_Update(fTimeDelta);
}

Layer* Layer::Create()
{
      return new Layer;
}

void Layer::Free()
{

 	__super::Free();

	for (auto& pGameObject : m_GameObjects)
		Safe_Release(pGameObject);

	m_GameObjects.clear();
}
