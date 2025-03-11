#include "Level.h"
#include "GameInstance.h"

Cevel::Cevel(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
	, m_pGameInstance{ CGameInstance::GetInstance()}
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
	Safe_AddRef(m_pGameInstance); // Level에서 GameInstance를 받아와서 사용할 수 있어서 Level에 한꺼번에 상속
}

HRESULT Cevel::Initialize()
{
	return S_OK;
}

void Cevel::Update(_float fTimeDelta)
{
}

HRESULT Cevel::Render()
{
	return S_OK;
}

void Cevel::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);
	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
