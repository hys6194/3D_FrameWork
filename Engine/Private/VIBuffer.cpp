#include "VIBuffer.h"

VIBuffer::VIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Component{ pDevice, pContext }
	, m_pVB{ Prototype.m_pVB }
	, m_pIB{ Prototype.m_pIB }
	, m_iVertexStride{ Prototype.m_iVertexStride }
	, m_iNumVertices{ Prototype.m_iNumVertices }
	, m_iIndexStride{ Prototype.m_iIndexStride }
	, m_iNumIndices{ Prototype.m_iNumIndices }
	, m_iNumVertexBuffers{ Prototype.m_iNumVertexBuffers }
	, m_eIndexFormat{ Prototype.m_eIndexFormat }
	, m_eTopology{ Prototype.m_eTopology }
{
	Safe_AddRef(m_pVB);
	Safe_AddRef(m_pIB);
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

HRESULT VIBuffer::Render()
{

	return S_OK;
}

HRESULT VIBuffer::Create_Buffer(ID3D11Buffer** ppOut)
{
	return S_OK;
}

void VIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
