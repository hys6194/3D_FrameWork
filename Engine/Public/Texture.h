#pragma once

#include "Component.h"

BEGIN(Engine)

class Texture : public Component
{
private:
	Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Texture(const Texture& Prototype);
	virtual ~Texture() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg);

private:
	_uint								m_iNumTextures = {};
	vector<ID3D11ShaderResourceView*>	m_vecSRV;

public:
	static Texture* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual Component* Clone(void* pArg) override;
	virtual void Free()override;

};

END