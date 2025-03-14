#pragma once

#include "pch.h"
#include "Base.h"

class MainApp : public Base
{
private:
	MainApp();
	virtual ~MainApp() = default;

public:
	HRESULT							Initialize();
	void							Update();	
	HRESULT							Render();



private:
	ID3D11Device*					m_pDevice = { nullptr };
	ID3D11DeviceContext*			m_pContext = { nullptr };

public:
	static MainApp*					Create();
	virtual void					Free() override;

};

