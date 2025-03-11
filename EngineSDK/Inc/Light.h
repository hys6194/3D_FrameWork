#pragma once

#include "Base.h"

class Light : public CBase
{
private:
	Light(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~Light() = default;

public:
	const LIGHT_DESC* Get_LightDesc() const {
		return &m_LightDesc;
	}

public:
	HRESULT Initialize(const LIGHT_DESC& pDesc);

private:
	ID3D11Device*			m_pDevice = { nullptr };
	ID3D11DeviceContext*	m_pContext = { nullptr };
	LIGHT_DESC				m_LightDesc = {};

public:
	static Light* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const LIGHT_DESC& pDesc);
	virtual void Free() override;

};

