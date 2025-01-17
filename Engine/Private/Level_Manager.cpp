#include "Level_Manager.h"
#include "GameInstance.h"
#include "Level.h"

Level_Manager::Level_Manager()
    : m_pGameInstance {GameInstance::GetInstance()}
{
    Safe_AddRef(m_pGameInstance);
}

HRESULT Level_Manager::Initialize()
{
    return S_OK;
}

void Level_Manager::Update(_float fTimeDelta)
{
    if (nullptr != m_pCurrentLevel)
        m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT Level_Manager::Render()
{
    return S_OK;
}

HRESULT Level_Manager::Change_Level(_uint iLevelIndex, Level* pNewLevel)
{
    if (nullptr == pNewLevel)
        return E_FAIL;

    // 이전 레벨을 삭제하고 최근 레벨을 대입해야함
    if (nullptr != m_pCurrentLevel)
    {
        Safe_Release(m_pCurrentLevel);

        m_pGameInstance->Clear(m_iCurrentLevelIndex);
    }

    m_pCurrentLevel = pNewLevel;

    m_iCurrentLevelIndex = iLevelIndex;

    return S_OK;
}

Level_Manager* Level_Manager::Create()
{
    Level_Manager* pInstance = new Level_Manager();
    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed To Created : Level_Manager");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Level_Manager::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
}
