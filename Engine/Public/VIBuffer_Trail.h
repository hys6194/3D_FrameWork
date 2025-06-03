#pragma once

#include "VIBuffer.h"

BEGIN (Engine)

class ENGINE_DLL CVIBuffer_Trail final : public CVIBuffer
{
protected:
	CVIBuffer_Trail(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Trail(const CVIBuffer_Trail& Prototype);
	virtual ~CVIBuffer_Trail() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;

public:
	HRESULT Trail(_float fTimeDelta, void* pArg);
	HRESULT Sword_Trail(_float fTimeDelta, void* pArg);
	HRESULT Motion_Trail(_float fTimeDelta, void* pArg);

private:
	_float3				m_fPrePos1;
	_float3				m_fPrePos2;

	_float				m_fTotalTime;

public:
	static CVIBuffer_Trail* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;

};

END