#include "State.h"
#include "GameInstance.h"

CState::CState(CGameObject* pOwner, CGameObject* pAnimOwner, CGameInstance* pGameInstance)
	: m_pOwner{ pOwner }
	, m_pAnimOwner{ pAnimOwner}
	, m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pGameInstance);
}


void CState::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
}
