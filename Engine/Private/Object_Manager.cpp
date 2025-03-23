#include "Object_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "Component.h"
#include "Layer.h"

CObject_Manager::CObject_Manager()
    : m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevel)
{
    m_iNumLevels = iNumLevel;

    m_pLayers = new map<const _wstring, CLayer*>[m_iNumLevels];

    return S_OK;
}

HRESULT CObject_Manager::Add_GameObject(_uint iPrototypeLevelIndex, const wstring& strPrototypeTag, _uint iLevelIndex, const _wstring& strLayerTag, void* pArg)
{
    if (iLevelIndex >= m_iNumLevels)
        return E_FAIL;

    // 복제할 원형 객체를 찾아서 복제해온다
    CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
    if (nullptr == pGameObject)
        return E_FAIL;

    CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);

    if (nullptr == pLayer)
    {
        pLayer = CLayer::Create();

        pLayer->Add_GameObject(pGameObject);

        m_pLayers[iLevelIndex].emplace(strLayerTag, pLayer);
    }
    else
        pLayer->Add_GameObject(pGameObject);

    return S_OK;
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
    for (size_t i = 0; i < m_iNumLevels; i++)
    {
        for (auto& Pair : m_pLayers[i])
            Pair.second->Priority_Update(fTimeDelta);
    }
}

void CObject_Manager::Update(_float fTimeDelta)
{
    for (size_t i = 0; i < m_iNumLevels; i++)
    {
        for (auto& Pair : m_pLayers[i])
            Pair.second->Update(fTimeDelta);
    }
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
    for (size_t i = 0; i < m_iNumLevels; i++)
    {
        for (auto& Pair : m_pLayers[i])
            Pair.second->Late_Update(fTimeDelta);
    }
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
    if (iLevelIndex >= m_iNumLevels)
        return;

    for (auto& Pair : m_pLayers[iLevelIndex])
        Safe_Release(Pair.second);

    m_pLayers[iLevelIndex].clear();
}

CLayer* CObject_Manager::Find_Layer(_uint iLevelIndex, const _wstring& strLayerTag)
{
    auto iter = m_pLayers[iLevelIndex].find(strLayerTag);
    
    if (iter == m_pLayers[iLevelIndex].end())
        return nullptr; 
    
    return iter->second;
}

HRESULT CObject_Manager::Delete_LastLayer(_uint iLevelIndex, const _wstring& strLayerTag)
{
    auto iter = m_pLayers[iLevelIndex].find(strLayerTag);

    //if (iter == m_pLayers[iLevelIndex].end())
    //    return E_FAIL;
    //m_pLayers[iLevelIndex].erase(iter->first);

    iter->second->Delete_LastObject();

    return S_OK;
}

CGameObject* CObject_Manager::Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, const _tchar* strObjectTag)
{
    CGameObject* pGameObeject = Find_Layer(iLevelIndex, strLayerTag)->Find_Object(strObjectTag);
    
    if(nullptr == pGameObeject)
        return nullptr;

    return pGameObeject;
}

CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
    CComponent* pComponent = Find_Layer(iLevelIndex, strLayerTag)->Find_Component(strComponentTag, iIndex);

    if (nullptr == pComponent)
        return nullptr;

    return pComponent;
}

CObject_Manager* CObject_Manager::Create(_uint iNumLevels)
{
    CObject_Manager* pInstance = new CObject_Manager();

    if (FAILED(pInstance->Initialize(iNumLevels)))
    {
        MSG_BOX("Failed to Created : Object_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CObject_Manager::Free()
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
