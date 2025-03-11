#pragma once

#include "Base.h" 

BEGIN(Engine)

class ENGINE_DLL Component abstract : public CBase
{
protected:
	Component(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Component(const Component& Prototype);
	virtual ~Component() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };

public:
	virtual Component* Clone(void* pArg) = 0;
	virtual void Free() override;

};

END