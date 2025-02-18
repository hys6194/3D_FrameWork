#include "VIBuffer.h"

VIBuffer::VIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: Component{ pDevice, pContext }
{
}

VIBuffer::VIBuffer(const VIBuffer& Prototype)
	: Component{ Prototype }
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
	if (nullptr == m_pContext)
		return E_FAIL;

	m_pContext->DrawIndexed(m_iNumIndices, 0, 0);

	return S_OK;
}

HRESULT VIBuffer::Apply_Input_Assembler()
{

	// 본래 렌더에서 호출하던 코드들이었음
	// 왜 분리하였는가?
	// Render에서 바인딩하고 Render하는 구조라서 한번 바인딩하고 여러번 Render하기 위해서 분리하였음

	ID3D11Buffer* pVertexBuffers[] =
	{
		// 추가적으로 넣고자 하는 버퍼를 계속 넣을 수 있음
		m_pVB,
	};

	_uint iStrides[] =
	{
		m_iVertexStride,
	};

	_uint Offsets[] =
	{
		0,
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, Offsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);


	return S_OK;
}

HRESULT VIBuffer::Create_Buffer(ID3D11Buffer** ppOut)
{
	return m_pDevice->CreateBuffer(&m_BufferDesc, &m_InitialData, ppOut);
}

void VIBuffer::Free()
{
	__super::Free();

	Safe_Release(m_pVB);
	Safe_Release(m_pIB);
}
