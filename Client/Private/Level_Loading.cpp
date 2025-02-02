#include "Level_Loading.h"
#include "Loader.h"

#include "Level_Logo.h" 
#include "Level_GamePlay.h" 
#include "GameInstance.h"

Level_Loading::Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:Level{ pDevice , pContext }
{
}

HRESULT Level_Loading::Initialize(LEVEL eLevelID)
{
	m_eLevelID = eLevelID;

	m_pLoader = Loader::Create(m_pDevice, m_pContext, eLevelID);

	if (nullptr == m_pLoader)
	{
		return E_FAIL;
	}

	return S_OK;
}

void Level_Loading::Update(_float fTimeDelta)
{
	// 여기서 레벨을 Create하여 리소스들을 다 읽어오게 되면 다음 레벨을 연다

	m_pLoader->Show_LoadingState();

	if (true == m_pLoader->isFinished())
	{
		if (GetKeyState(VK_RETURN) & 0x8000)
		{
			Level* pLevel = { nullptr };

			switch (m_eLevelID)
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

			if (FAILED(m_pGameInstance->Open_Level(m_eLevelID, pLevel)))
				return;

		}
	}
}

HRESULT Level_Loading::Render()
{
	return S_OK;
}

Level_Loading* Level_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
{
	Level_Loading* pInstance = new Level_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eLevelID)))
	{
		MSG_BOX("Failed Create Level_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Level_Loading::Free()
{
	__super::Free();
}
