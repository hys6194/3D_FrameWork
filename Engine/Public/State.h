#pragma once

#include "Base.h"

BEGIN(Engine)

class ENGINE_DLL State abstract: public Base
{
protected:
	State(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	virtual ~State() = default;

public:
	virtual HRESULT Enter_State() = 0;
	virtual void Update_State() = 0;
	virtual HRESULT Exit_State() = 0;

protected:
	_uint					m_iState = {};
	ID3D11Device* m_pDevice = { nullptr };
	ID3D11DeviceContext* m_pContext = { nullptr };


public:
	virtual void Free() override;
};

END