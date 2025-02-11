#pragma once

#include "Component.h"

BEGIN (Engine)

class ENGINE_DLL VIBuffer abstract : public Component
{
protected:
	VIBuffer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	VIBuffer(const VIBuffer& Prototype);
	virtual ~VIBuffer() = default;

public:
	HRESULT Initialize_Prototype() override;
	HRESULT Initialize(void* pArg) override;

protected:
	ID3D11Buffer* m_pVB = { nullptr };
	ID3D11Buffer* m_pIB = { nullptr };

public:
	virtual Component* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END