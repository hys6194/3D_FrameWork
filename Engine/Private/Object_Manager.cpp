#include "Object_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Layer.h"

Object_Manager::Object_Manager()
    : m_pGameInstance{ GameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT Object_Manager::Initialize(_uint iNumLevel)
{
    m_iNumLevels = iNumLevel;

    m_pLayers = new map<const _wstring, Layer*>[m_iNumLevels];

    return S_OK;
}

HRESULT Object_Manager::Add_GameObject(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
    if (iLevelIndex >= m_iNumLevels)
        return E_FAIL;

    // 복제할 원형 객체를 찾아서 복제해온다
    GameObject* pGameObject = dynamic_cast<GameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
    if (nullptr == pGameObject)
        return E_FAIL;

    Layer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

    if (nullptr == pLayer)
    {
        pLayer = Layer::Create();

        pLayer->Add_GameObject(pGameObject);

        m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
    }
    else
        pLayer->Add_GameObject(pGameObject);

    return S_OK;
}

void Object_Manager::Priority_Update(_float fTimeDelta)
{
    for (size_t i = 0; i < m_iNumLevels; i++)
    {
        for (auto& Pair : m_pLayers[i])
            Pair.second->Priority_Update(fTimeDelta);
    }


}

void Object_Manager::Update(_float fTimeDelta)
{
    for (size_t i = 0; i < m_iNumLevels; i++)
    {
        for (auto& Pair : m_pLayers[i])
            Pair.second->Update(fTimeDelta);
    }
}

void Object_Manager::Late_Update(_float fTimeDelta)
{
    for (size_t i = 0; i < m_iNumLevels; i++)
    {
        for (auto& Pair : m_pLayers[i])
            Pair.second->Late_Update(fTimeDelta);
    }
}

void Object_Manager::Clear(_uint iLevelIndex)
{
    if (iLevelIndex >= m_iNumLevels)
        return;

    for (auto& Pair : m_pLayers[iLevelIndex])
        Safe_Release(Pair.second);

    m_pLayers[iLevelIndex].clear();
}

Layer* Object_Manager::Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
    auto iter = m_pLayers[iLevelIndex].find(strLayerTag);
    
    if (iter == m_pLayers[iLevelIndex].end())
        return nullptr; 
    
    return iter->second;
}

GameObject* Object_Manager::Get_Object(_uint iLevelIndex, const _wstring& strLayerTag, const _tchar& strObjectTag)
{
    Layer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

    

    return nullptr;
}

Object_Manager* Object_Manager::Create(_uint iNumLevels)
{
    Object_Manager* pInstance = new Object_Manager();

    if (FAILED(pInstance->Initialize(iNumLevels)))
    {
        MSG_BOX("Failed to Created : Object_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Object_Manager::Free()
{
    __super::Free();

    for (size_t i = 0; i < m_iNumLevels; ++i)
    {
        for (auto& Pair : m_pLayers[i])
            Safe_Release(Pair.second);

        m_pLayers[i].clear();
    }
    Safe_Delete_Array(m_pLayers);

    Safe_Release(m_pGameInstance);
}
