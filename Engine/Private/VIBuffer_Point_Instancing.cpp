#include "VIBuffer_Point_Instancing.h"
#include "GameInstance.h"

CVIBuffer_Point_Instancing::CVIBuffer_Point_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CVIBuffer_Particle{ pDevice, pContext }
{
}

CVIBuffer_Point_Instancing::CVIBuffer_Point_Instancing(const CVIBuffer_Point_Instancing& Prototype)
    : CVIBuffer_Particle{ Prototype }
{
}

HRESULT CVIBuffer_Point_Instancing::Initialize_Prototype(CVIBuffer_Instancing::INSTANCE_DESC* pDesc)
{
	CVIBuffer_Particle::INSTANCE_PARTICLE_DESC* pInstanceDesc = static_cast<CVIBuffer_Particle::INSTANCE_PARTICLE_DESC*>(pDesc);

	m_vPivot = pInstanceDesc->vPivot;
	m_iNumIndexPerInstance = 6;
	m_iNumInstance = pInstanceDesc->iNumInstances;
	m_iInstanceStride = sizeof(INSTVTX);
	m_isLoop = pInstanceDesc->isLoop;


	m_iVertexStride = sizeof(VTXPOS);
	m_iNumVertices = 1;
	m_iIndexStride = 2;
	m_iNumIndices = m_iNumInstance;
	m_iNumVertexBuffers = 3;
	m_eIndexFormat = DXGI_FORMAT_R16_UINT;
	m_eTopology = D3D_PRIMITIVE_TOPOLOGY_POINTLIST;

#pragma region VERTEXBUFFER
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iVertexStride * m_iNumVertices;
	m_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iVertexStride;
	m_BufferDesc.CPUAccessFlags = 0;
	m_BufferDesc.MiscFlags = 0;

	VTXPOS* pVertices = new VTXPOS();
	ZeroMemory(pVertices, sizeof(VTXPOS));

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
		m_pParticleSpeeds[i] = m_pGameInstance->Random(pInstanceDesc->vSpeed.x, pInstanceDesc->vSpeed.y);
		m_pParticleInstanceVertices[i].vLifeTime = _float2(m_pGameInstance->Random(pInstanceDesc->vLifeTime.x, pInstanceDesc->vLifeTime.y), 0.f);
	}

#pragma endregion

#pragma region INSTANCEBUFFER
	m_pInstanceVertices = new INSTVTX[m_iNumInstance];
	ZeroMemory(m_pInstanceVertices, sizeof(INSTVTX) * m_iNumInstance);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		_float		fScale = m_pGameInstance->Random(pInstanceDesc->vSize.x, pInstanceDesc->vSize.y);

		m_pInstanceVertices[i].vRight = _float4(fScale, 0.f, 0.f, 0.f);
		m_pInstanceVertices[i].vUp = _float4(0.f, fScale, 0.f, 0.f);
		m_pInstanceVertices[i].vLook = _float4(0.f, 0.f, fScale, 0.f);
		m_pInstanceVertices[i].vTranslation = _float4(
			m_pGameInstance->Random(pInstanceDesc->vCenter.x - pInstanceDesc->vRange.x * 0.5f, pInstanceDesc->vCenter.x + pInstanceDesc->vRange.x * 0.5f),
			m_pGameInstance->Random(pInstanceDesc->vCenter.y - pInstanceDesc->vRange.y * 0.5f, pInstanceDesc->vCenter.y + pInstanceDesc->vRange.y * 0.5f),
			m_pGameInstance->Random(pInstanceDesc->vCenter.z - pInstanceDesc->vRange.z * 0.5f, pInstanceDesc->vCenter.z + pInstanceDesc->vRange.z * 0.5f),
			1.f);


	}

#pragma endregion


	return S_OK;
}

HRESULT CVIBuffer_Point_Instancing::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

CVIBuffer_Point_Instancing* CVIBuffer_Point_Instancing::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CVIBuffer_Instancing::INSTANCE_DESC* pDesc)
{
	CVIBuffer_Point_Instancing* pInstance = new CVIBuffer_Point_Instancing(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(pDesc)))
	{
		MSG_BOX("Failed To Created : CVIBuffer_Point_Instancing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Point_Instancing::Clone(void* pArg)
{
	CComponent* pInstance = new CVIBuffer_Point_Instancing(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CVIBuffer_Point_Instancing");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Point_Instancing::Free()
{
	__super::Free();


}
