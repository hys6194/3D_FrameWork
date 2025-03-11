#include "State.h"
#include "GameInstance.h"

CState::CState(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CGameObject* pOwner, CGameObject* pAnimOwner, CGameInstance* pGameInstance)
	: m_pDevice{ pDevice }
	, m_pContext { pContext }
	, m_pOwner{ pOwner }
	, m_pAnimOwner{ pAnimOwner}
	, m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance);
}


void CState::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
	Safe_Release(m_pGameInstance);
}
