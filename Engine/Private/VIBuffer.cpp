#include "VIBuffer.h"

VIBuffer::VIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Component{ pDevice, pContext }
{
}

VIBuffer::VIBuffer(const VIBuffer& Prototype)
	: Component{ Prototype }
{
}

HRESULT VIBuffer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT VIBuffer::Initialize(void* pArg)
{
	return S_OK;
}

void VIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
