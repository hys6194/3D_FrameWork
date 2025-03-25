#include "VIBuffer_Particle.h"

CVIBuffer_Particle::CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:CVIBuffer_Instancing{ pDevice, pContext }
{
}

CVIBuffer_Particle::CVIBuffer_Particle(const CVIBuffer_Particle& Prototype)
	:CVIBuffer_Instancing{ Prototype }
{
}

HRESULT CVIBuffer_Particle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CVIBuffer_Particle::Initialize(void* pArg)
{
	return S_OK;
}

void CVIBuffer_Particle::Free()
{
	__super::Free();
}
