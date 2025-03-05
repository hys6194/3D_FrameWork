#include "State.h"

State::State(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, GameObject* pOwner)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pOwner{ pOwner }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}


void State::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
