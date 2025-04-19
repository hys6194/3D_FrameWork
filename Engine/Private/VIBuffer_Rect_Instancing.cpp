#include "VIBuffer_Particle.h"


#include "GameInstance.h"

CVIBuffer_Rect_Instancing::CVIBuffer_Rect_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer_Particle{ pDevice, pContext }
{
}

CVIBuffer_Rect_Instancing::CVIBuffer_Rect_Instancing(const CVIBuffer_Rect_Instancing& Prototype)
	:CVIBuffer_Particle{ Prototype }
{
}

HRESULT CVIBuffer_Rect_Instancing::Initialize_Prototype(CVIBuffer_Instancing::INSTANCE_DESC* pInstDesc)
{
	CVIBuffer_Particle::INSTANCE_PARTICLE_DESC* pDesc = static_cast<CVIBuffer_Particle::INSTANCE_PARTICLE_DESC*>(pInstDesc);

	// 원래 사각형을 그릴 때, 버텍스 정점 6개를 사용했기 때문에 6으로 선언
	m_iNumIndexPerInstance = 6;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iNumVertices = 4;
	m_iIndexStride = 2;

	//Desc으로 받아온 생성할 개수만큼  버퍼를 할당 및 생성
	m_iNumInstance = pDesc->iNumInstances;

	// 사각형을 그릴 것이므로 6 * num
	m_iNumIndices = 6 * m_iNumInstance;
	m_iNumVertexBuffers = 3;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

#pragma region VERTEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXPOSTEX* pVertices = new VTXPOSTEX[m_iNumVertices];
	ZeroMemory(pVertices, sizeof(VTXPOSTEX) * m_iNumVertices);

	pVertices[0].vPosition = _float3(-0.5f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(0.5f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(0.5f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(-0.5f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pVertices;

	if (FAILED(__super::Create_Buffer(&m_pVB)))
		return E_FAIL;

	Safe_Delete_Array(pVertices);

#pragma endregion

#pragma region INDEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iIndexStride * m_iNumIndices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iIndexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	_ushort* pIndices = new _ushort[m_iNumIndices];
	ZeroMemory(pIndices, sizeof(_ushort) * m_iNumIndices);

	_uint		iNumIndices = {};

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 1;
		pIndices[iNumIndices++] = 2;

		pIndices[iNumIndices++] = 0;
		pIndices[iNumIndices++] = 2;
		pIndices[iNumIndices++] = 3;
	}

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = pIndices;

	if (FAILED(__super::Create_Buffer(&m_pIB)))
		return E_FAIL;

	Safe_Delete_Array(pIndices);

#pragma endregion

#pragma region PARTICLE_INSTANCEBUFFER
	m_pParticleSpeeds = new _float[m_iNumInstance];
	m_iParticleInstanceStride = sizeof(VTXPARTICLE);

	m_pParticleInstanceVertices = new VTXPARTICLE[m_iNumInstance];
	ZeroMemory(m_pParticleInstanceVertices, sizeof(VTXPARTICLE) * m_iNumInstance);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_pParticleSpeeds[i] = m_pGameInstance->Random(pDesc->vSpeed.x, pDesc->vSpeed.y);
		m_pParticleInstanceVertices[i].vLifeTime = _float2(m_pGameInstance->Random(pDesc->vLifeTime.x, pDesc->vLifeTime.y), 0.f);
	}

#pragma endregion

#pragma region INSTANCEBUFFER

	m_iInstanceStride = sizeof(INSTVTX);

	m_pInstanceVertices = new INSTVTX[m_iNumInstance];
	ZeroMemory(m_pInstanceVertices, sizeof(INSTVTX) * m_iNumInstance);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		m_pInstanceVertices[i].vRight = _float4(1.f, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vUp = _float4(0.f, 1.f, 0.f, 0.f);
		m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, 1.f, 0.f);
		// 생성지점 랜덤으러 설정한거 최소한 겹치지 않게
		m_pInstanceVertices[i].vTranslation = _float4
		(
			m_pGameInstance->Random(pDesc->vCenter.x - pDesc->vRange.x * 0.5f, pDesc->vCenter.x + pDesc->vRange.x * 0.5f),
			m_pGameInstance->Random(pDesc->vCenter.y - pDesc->vRange.y * 0.5f, pDesc->vCenter.y + pDesc->vRange.y * 0.5f),
			m_pGameInstance->Random(pDesc->vCenter.z - pDesc->vRange.z * 0.5f, pDesc->vCenter.z + pDesc->vRange.z * 0.5f),
			1.f
		);
	}

#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Rect_Instancing::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Rect_Instancing* CVIBuffer_Rect_Instancing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CVIBuffer_Instancing::INSTANCE_DESC* pDesc)
{
	CVIBuffer_Rect_Instancing* pInstance = new CVIBuffer_Rect_Instancing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pDesc)))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Rect_Instancing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Rect_Instancing::Clone(void* pArg)
{
	CComponent* pInstance = new CVIBuffer_Rect_Instancing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_Rect_Instancing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Rect_Instancing::Free()
{
	__super::Free();

	Safe_Release(m_pVBParticle);

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pParticleSpeeds);
		Safe_Delete_Array(m_pParticleInstanceVertices);
	}

}
