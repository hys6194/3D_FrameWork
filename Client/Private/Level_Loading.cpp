#include "Level_Loading.h"
#include "Loader.h"

// Level을 상속받는 클래스가 생기면 주기적으로 추가해야 함
#include "Level_Logo.h"
#include "Level_GamePlay.h"
#include "GameInstance.h"

Level_Loading::Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Level{ pDevice, pContext }
{
}

HRESULT Level_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	// 로딩레벨의 본래 역할인 다음 레벨을 위한 자원을 준비하는 작업을 여기서 수행
	m_pLoader = Loader::Create(m_pDevice, m_pContext, m_eNextLevelID);
	if (FAILED(nullptr == m_pLoader))
		return E_FAIL;

	//로딩 레벨의 장면을 담당해줄 객체들을 여기서 만들어 줄 것
	return S_OK;
}

void Level_Loading::Update(_float fTimeDelta)
{
	m_pLoader->Show_LoadingState();

	if(true == m_pLoader->IsFinished())
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			Level* pLevel = { nullptr };

			switch (m_eNextLevelID)
			{
			case LEVEL_LOGO:
				pLevel = Level_Logo::Create(m_pDevice, m_pContext);
				break;
			case LEVEL_GAMEPLAY:
				pLevel = Level_GamePlay::Create(m_pDevice, m_pContext);
				break;
			default:
				break;
			}

			if (nullptr == pLevel)
				return;

			if (FAILED(m_pGameInstance->Open_Level(m_eNextLevelID, pLevel)))
				return;
		}
	}

}

HRESULT Level_Loading::Render()
{
	return S_OK;
}

Level_Loading* Level_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eNextLevelID)
{
	Level_Loading* pInstance = new Level_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eNextLevelID)))
	{
		MSG_BOX("Failed To Created : Level_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Level_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
