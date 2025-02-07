#pragma once

#include "Base.h" 

BEGIN(Engine)

class Component abstract : public Base
{
protected:
	Component(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Component(const Component& Prototype);
	virtual ~Component() = default;

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual Component* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END