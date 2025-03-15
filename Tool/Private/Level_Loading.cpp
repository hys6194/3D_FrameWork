#include "Level_Loading.h"
#include "Loader.h"

#include "Tool_Level.h"

HRESULT Level_Loading::Initialize(LEVEL eLevelID)
{
	m_eLevelID = eLevelID;

	m_pLoader = Loader::Create(m_pDevice, m_pContext, eLevelID);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	if (true == m_pLoader->isFinished())
	{
		CLevel* pLevel = { nullptr };

		switch (m_eLevelID)
		{
		case LEVEL_TOOL:
			pLevel = Tool_Level::Create(m_pDevice, m_pContext);
			break;
		default:
			break;
		}

		if (nullptr == pLevel) return;

		if (FAILED(m_pGameInstance->Open_Level(m_eLevelID, pLevel)))
			return;
		//if (GetKeyState(VK_RETURN) & 0x8000)
		//{
		//
		//}
	}

	return S_OK;
}

void Level_Loading::Update(_float fTimeDelta)
{
	m_pLoader->Show_LoadingState();


}

HRESULT Level_Loading::Render()
{
	return E_NOTIMPL;
}

Level_Loading* Level_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
{
	return nullptr;
}

void Level_Loading::Free()
{
}
