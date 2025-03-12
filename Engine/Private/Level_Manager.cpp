#include "Level_Manager.h"
#include "GameInstance.h"
#include "Level.h"

CLevel_Manager::CLevel_Manager()
	: m_pGameInstance {CGameInstance::GetInstance()}
{
}

HRESULT CLevel_Manager::Initialize()
{
	return S_OK;
}

void CLevel_Manager::Update(_float fTimeDelta)
{
	if (nullptr != m_pCurrentLevel)
		m_pCurrentLevel->Update(fTimeDelta);
}

HRESULT CLevel_Manager::Render()
{
	return S_OK;
}

HRESULT CLevel_Manager::Change_Level(_uint iLevelIndex, CLevel* pNewLevel)
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

CLevel_Manager* CLevel_Manager::Create()
{
	CLevel_Manager* pInstance = new CLevel_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed Create Level_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pCurrentLevel);
}
