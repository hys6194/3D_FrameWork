#include "Component.h"

Component::Component(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: m_pDevice{ pDevice }
	, m_pContext { pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

Component::Component(const Component& Prototype)
	: m_pDevice { Prototype.m_pDevice }
	, m_pContext{ Prototype.m_pContext }
{
	Safe_AddRef(m_pDevice);
	Safe_AddRef(m_pContext);
}

HRESULT Component::Initialize_Prototype()
{
	return E_NOTIMPL;
}

HRESULT Component::Initialize(void* pArg)
{
	return E_NOTIMPL;
}

void Component::Free()
{
	__super::Free();

	Safe_Release(m_pContext);
	Safe_Release(m_pDevice);
}
