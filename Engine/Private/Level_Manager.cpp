#include "Level_Manager.h"
#include "GameInstance.h"
#include "Level.h"

Level_Manager::Level_Manager()
	: m_pGameInstance {GameInstance::GetInstance()}
{
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
	// 인자값으로 받아온 레벨이 nullptr이면 레벨을 변경하지 않는다
	if (nullptr == pNewLevel)
		return E_FAIL;

	// 이전 레벨에서 생성한 객체들을 정리한다
	if (nullptr != m_pCurrentLevel)
	{
		Safe_Release(m_pCurrentLevel);

		// enum값으로 받아올 LEVEL을 통해 특정 레벨을 정리
		m_pGameInstance->Clear(m_iCurrentLevelIndex);
	}

	// 레벨 변경
	m_pCurrentLevel = pNewLevel;

	m_iCurrentLevelIndex = iLevelIndex;

	return S_OK;
}

Level_Manager* Level_Manager::Create()
{
	Level_Manager* pInstance = new Level_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Level_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Level_Manager::Free()
{
}
