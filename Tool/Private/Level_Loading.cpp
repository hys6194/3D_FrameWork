#include "Level_Loading.h"
#include "Loader.h"

#include "Tool_Level.h"
#include "GameInstance.h"

Level_Loading::Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel{ pDevice , pContext }
{
}

HRESULT Level_Loading::Initialize(LEVEL eLevelID)
{
	m_eLevelID = eLevelID;

	m_pLoader = Loader::Create(m_pDevice, m_pContext, eLevelID);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	return S_OK;
}

void Level_Loading::Update(_float fTimeDelta)
{
	m_pLoader->Show_LoadingState();

	if (true == m_pLoader->isFinished())
	{
		CLevel* pLevel = { nullptr };

		pLevel = Tool_Level::Create(m_pDevice, m_pContext);

		if (nullptr == pLevel) return;

		if (FAILED(m_pGameInstance->Open_Level(m_eLevelID, pLevel)))
			return;
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

	Safe_Release(m_pLoader);
}
