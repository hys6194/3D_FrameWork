#include "Level_Loading.h"
#include "Loader.h"

#include "Level_Logo.h"
#include "Level_GamePlay.h"

Level_Loading::Level_Loading(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Level{ pDevice, pContext }
{
}

HRESULT Level_Loading::Initialize(LEVEL eNextLevelID)
{
	m_eNextLevelID = eNextLevelID;

	// 레벨로딩 클래스 가 loader클래스를 호출해서 어떤 클래스를 불러올 지 결정한다
	FAILED_CHECK_RETURN(m_pLoader = Loader::Create(m_pDevice, m_pContext, m_eNextLevelID), E_FAIL);

	return S_OK;
}

void Level_Loading::Update(_float fTimeDelta)
{

	m_pLoader->Show_LoadingState();

	// 이벤트가 발생 시, 씬을 변경하는 구조를 만들 것
	// 일단 키 입력으로 이벤트를 발생시킬 것

	if (GetKeyState(VK_RETURN))
	{
		Level* pLevel = { nullptr };

		switch (m_eNextLevelID)
		{
		case LEVEL_STATIC:
			break;
		case LEVEL_LOGO:
			pLevel = Level_Logo::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_GAMEPLAY:
			pLevel = Level_GamePlay::Create(m_pDevice, m_pContext);
			break;
		case LEVEL_LOADING:
			break;
		case LEVEL_END:
			break;
		default:
			break;
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
}
