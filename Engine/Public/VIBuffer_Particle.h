#pragma once

#include "VIBuffer_Instancing.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Particle abstract : public CVIBuffer_Instancing
{
public:
	typedef struct tagInstanceParticle : public CVIBuffer_Instancing::INSTANCE_DESC
	{
		_float2 vSpeed;
		_float2 vLifeTime;
	}INSTANCE_PARTICLE_DESC;

protected:
	CVIBuffer_Particle(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Particle(const CVIBuffer_Particle& Prototype);
	virtual ~CVIBuffer_Particle() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

public:
	virtual HRESULT Bind_Input_Assembler() override;

public:
	void Drop(_float fTimeDelta);
	void Spread(_float fTimeDelta);

protected:
	ID3D11Buffer*		m_pVBParticle = { nullptr };

	_float*				m_pParticleSpeeds = { nullptr };
	_uint				m_iParticleInstanceStride = {};
	VTXPARTICLE*		m_pParticleInstanceVertices = { nullptr };

public:
	virtual CComponent* Clone(void* pArg) = 0;
	virtual void Free() override;
};

END