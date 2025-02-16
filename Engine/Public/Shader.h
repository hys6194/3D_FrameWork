#pragma once

#include "Component.h"

BEGIN (Engine)

class Shader final : public Component
{
private:
	Shader(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Shader(const Shader& Prototype);
	virtual ~Shader() = default; 

public:
	virtual HRESULT Initialize_Prototype(const _tchar* pShaderFilePath);
	virtual HRESULT Initialize(void* pArg) override;

private:
	LPD3D11EFFECT*				m_pEffect = { nullptr };

public:
	static Shader* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pShaderFilePath);
	virtual Component* Clone(void* pArg) override;
	virtual void Free() override;
};

END