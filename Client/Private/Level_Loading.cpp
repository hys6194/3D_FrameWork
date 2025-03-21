#include "Level_Loading.h"
#include "Loader.h"

#include "Level_Logo.h" 
#include "Level_Menu.h" 
#include "Level_GamePlay.h" 
#include "GameInstance.h"

CLevel_Loading::CLevel_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CLevel{ pDevice , pContext }
{
}

HRESULT CLevel_Loading::Initialize(LEVEL eLevelID)
{
	m_eLevelID = eLevelID;

	m_pLoader = CLoader::Create(m_pDevice, m_pContext, eLevelID);
	NULL_CHECK_RETURN(m_pLoader, E_FAIL);

	return S_OK;
}

void CLevel_Loading::Update(_float fTimeDelta)
{
	// 여기서 레벨을 Create하여 리소스들을 다 읽어오게 되면 다음 레벨을 연다

	m_pLoader->Show_LoadingState();

	if (true == m_pLoader->isFinished())
	{
		CLevel* pLevel = { nullptr };

		switch (m_eLevelID)
		{
		case LEVEL_LOGO:
			pLevel = CLevel_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_MENU:
			pLevel = CLevel_Menu::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_GAMEPLAY:
			pLevel = CLevel_GamePlay::Create(m_pDevice, m_pContext);
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
}

HRESULT CLevel_Loading::Render()
{
	return S_OK;
}

HRESULT CLevel_Loading::Ready_Loading_Textures()
{
	//CUIObject::




	return S_OK;
}

CLevel_Loading* CLevel_Loading::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevelID)
{
	CLevel_Loading* pInstance = new CLevel_Loading(pDevice, pContext);

	if (FAILED(pInstance->Initialize(eLevelID)))
	{
		MSG_BOX("Failed Create Level_Loading");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLevel_Loading::Free()
{
	__super::Free();

	Safe_Release(m_pLoader);
}
