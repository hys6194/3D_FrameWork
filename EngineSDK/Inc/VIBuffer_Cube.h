#pragma once

#include "VIBuffer.h"

BEGIN (Engine)

class ENGINE_DLL VIBuffer_Cube final : public VIBuffer
{
protected:
	VIBuffer_Cube(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	VIBuffer_Cube(const VIBuffer_Cube& Prototype);
	virtual ~VIBuffer_Cube() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;

public:
	static VIBuffer_Cube* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Component* Clone(void* pArg);
	virtual void Free() override;

};

END