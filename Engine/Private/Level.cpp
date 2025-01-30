#include "Level.h"

Level::Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice { pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT Level::Initialize()
{
	return S_OK;
}

void Level::Update(_float fTimeDelta)
{
}

HRESULT Level::Render()
{
	return S_OK;
}

void Level::Free()
{
	__super::Free();

	Safe_Release(m_pDevice);
	Safe_Release(m_pContext);
}
