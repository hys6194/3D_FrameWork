#pragma once

#include "VIBuffer_Particle.h"

BEGIN(Engine)

class ENGINE_DLL CVIBuffer_Rect_Instancing final : public CVIBuffer_Particle
{
private:
	CVIBuffer_Rect_Instancing(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	CVIBuffer_Rect_Instancing(const CVIBuffer_Rect_Instancing& Prototype);
	virtual ~CVIBuffer_Rect_Instancing() = default;

public:
	virtual HRESULT Initialize_Prototype(CVIBuffer_Instancing::INSTANCE_DESC* pInstDesc);
	virtual HRESULT Initialize(void* pArg) override;



public:
	static CVIBuffer_Rect_Instancing* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, CVIBuffer_Instancing::INSTANCE_DESC* pInstDesc);
	virtual CComponent* Clone(void* pArg);
	virtual void Free() override;
};

END