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
