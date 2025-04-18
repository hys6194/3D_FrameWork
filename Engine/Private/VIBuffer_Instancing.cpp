#include "VIBuffer_Instancing.h"

CVIBuffer_Instancing::CVIBuffer_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CVIBuffer{ pDevice, pContext }
{
}

CVIBuffer_Instancing::CVIBuffer_Instancing(const CVIBuffer_Instancing& Prototype)
	: CVIBuffer{ Prototype }
	, m_iInstanceStride{ Prototype.m_iInstanceStride }
	, m_iNumInstance{ Prototype.m_iNumInstance }
	, m_iNumIndexPerInstance {Prototype.m_iNumIndexPerInstance}
	, m_pInstanceVertices { Prototype.m_pInstanceVertices }
{
	// 얕은 복사를 하고 있는 중이므로 이에 대한 관리를 철저히 해야 한다
	//m_pInstanceVertices{ Prototype.m_pInstanceVertices }
}

HRESULT CVIBuffer_Instancing::Initialize_Prototype(CVIBuffer_Instancing::INSTANCE_DESC* pDesc)
{
	return S_OK;
}

HRESULT CVIBuffer_Instancing::Initialize(void* pArg)
{
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = m_pInstanceVertices;

	if (FAILED(__super::Create_Buffer(&m_pVBInstance)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Instancing::Render()
{
	if (nullptr == m_pContext)
		return E_FAIL;
	m_pContext->DrawIndexedInstanced(m_iNumIndexPerInstance, m_iNumInstance, 0, 0, 0);
	return S_OK;
}

HRESULT CVIBuffer_Instancing::Bind_Input_Assembler()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance
	};

	_uint iStride[] = {
		m_iVertexStride,
		m_iInstanceStride
	};

	_uint iOffset[] = {
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStride, iOffset);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

void CVIBuffer_Instancing::Free()
{
	__super::Free();

	if (false == m_isCloned)
		Safe_Delete_Array(m_pInstanceVertices);

	Safe_Release(m_pVBInstance);
}
