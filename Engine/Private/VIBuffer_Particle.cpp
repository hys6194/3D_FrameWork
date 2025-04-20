#include "VIBuffer_Particle.h"

CVIBuffer_Particle::CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer_Instancing{ pDevice, pContext }
{
}

CVIBuffer_Particle::CVIBuffer_Particle(const CVIBuffer_Particle& Prototype)
	:CVIBuffer_Instancing{ Prototype }
	, m_pVBParticle{ Prototype.m_pVBParticle }
	, m_vPivot{ Prototype.m_vPivot }
	, m_pParticleSpeeds{ Prototype.m_pParticleSpeeds }
	, m_iParticleInstanceStride{ Prototype.m_iParticleInstanceStride }
	, m_pParticleInstanceVertices{ Prototype.m_pParticleInstanceVertices }
	, m_isLoop{ Prototype.m_isLoop }
{
}

HRESULT CVIBuffer_Particle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Particle::Initialize(void* pArg)
{
	ZeroMemory(&m_BufferDesc, sizeof m_BufferDesc);
	m_BufferDesc.ByteWidth = m_iParticleInstanceStride * m_iNumInstance;
	m_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	m_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	m_BufferDesc.StructureByteStride = m_iParticleInstanceStride;
	m_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	m_BufferDesc.MiscFlags = 0;

	ZeroMemory(&m_InitialData, sizeof m_InitialData);
	m_InitialData.pSysMem = m_pParticleInstanceVertices;

	if (FAILED(__super::Create_Buffer(&m_pVBParticle)))
		return E_FAIL;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	return S_OK;
}

HRESULT CVIBuffer_Particle::Bind_Input_Assembler()
{
	ID3D11Buffer* pVertexBuffers[] = {
		m_pVB,
		m_pVBInstance,
		m_pVBParticle
	};

	_uint           iStrides[] = {
		m_iVertexStride,
		m_iInstanceStride,
		m_iParticleInstanceStride,
	};

	_uint           Offsets[] = {
		0,
		0,
		0
	};

	m_pContext->IASetVertexBuffers(0, m_iNumVertexBuffers, pVertexBuffers, iStrides, Offsets);
	m_pContext->IASetIndexBuffer(m_pIB, m_eIndexFormat, 0);
	m_pContext->IASetPrimitiveTopology(m_eTopology);

	return S_OK;
}

void CVIBuffer_Particle::Drop(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE			InstanceSubResource{};
	D3D11_MAPPED_SUBRESOURCE			ParticleSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &InstanceSubResource);
	m_pContext->Map(m_pVBParticle, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &ParticleSubResource);

	INSTVTX* pMatrices = static_cast<INSTVTX*>(InstanceSubResource.pData);
	VTXPARTICLE* pParticles = static_cast<VTXPARTICLE*>(ParticleSubResource.pData);

	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		// 시간에 따라 생존시간 증가
		// x값은 업데이트 주기 시간
		// y값은 생존시간이 끝나는 시간
		pParticles[i].vLifeTime.y += fTimeDelta;

		if (pParticles[i].vLifeTime.y >= pParticles[i].vLifeTime.x)
		{
			pParticles[i].vLifeTime.y = 0.f;
			pMatrices[i].vTranslation.y = m_pInstanceVertices[i].vTranslation.y;

		}

		pMatrices[i].vTranslation.y -= m_pParticleSpeeds[i] * fTimeDelta;

	}

	m_pContext->Unmap(m_pVBParticle, 0);
	m_pContext->Unmap(m_pVBInstance, 0);

	

}

void CVIBuffer_Particle::Spread(_float fTimeDelta)
{
	D3D11_MAPPED_SUBRESOURCE			InstanceSubResource{};
	D3D11_MAPPED_SUBRESOURCE			ParticleSubResource{};

	m_pContext->Map(m_pVBInstance, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &InstanceSubResource);
	m_pContext->Map(m_pVBParticle, 0, D3D11_MAP_WRITE_NO_OVERWRITE, 0, &ParticleSubResource);

	INSTVTX* pMatrices = static_cast<INSTVTX*>(InstanceSubResource.pData);
	VTXPARTICLE* pParticles = static_cast<VTXPARTICLE*>(ParticleSubResource.pData);


	for (size_t i = 0; i < m_iNumInstance; i++)
	{
		pParticles[i].vLifeTime.y += fTimeDelta;

		if (pParticles[i].vLifeTime.y >= pParticles[i].vLifeTime.x)
		{
			if (true == m_isLoop)
			{
				pParticles[i].vLifeTime.y = 0.f;
				pMatrices[i].vTranslation = m_pInstanceVertices[i].vTranslation;
			}
		}

		_vector		vMoveDir = XMLoadFloat4(&pMatrices[i].vTranslation) - XMVectorSetW(XMLoadFloat3(&m_vPivot), 1.f);
		XMStoreFloat4(&pMatrices[i].vTranslation, XMLoadFloat4(&pMatrices[i].vTranslation) + XMVector3Normalize(vMoveDir) * m_pParticleSpeeds[i] * fTimeDelta);


	}

	m_pContext->Unmap(m_pVBParticle, 0);
	m_pContext->Unmap(m_pVBInstance, 0);
}

void CVIBuffer_Particle::Free()
{
	__super::Free();

	Safe_Release(m_pVBParticle);

	if (false == m_isCloned)
	{
		Safe_Delete_Array(m_pParticleSpeeds);
		Safe_Delete_Array(m_pParticleInstanceVertices);
	}
}
