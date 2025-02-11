#pragma once

#include "VIBuffer.h"

BEGIN (Engine)

class ENGINE_DLL VIBuffer_Rect final : public VIBuffer
{
protected:
	VIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	VIBuffer_Rect(const VIBuffer_Rect& Prototype);
	virtual ~VIBuffer_Rect() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;

public:
	static VIBuffer_Rect* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Component* Clone(void* pArg);
	virtual void Free() override;

};

END