#pragma once

#include"Base.h"

BEGIN(Engine)

class ENGINE_DLL State abstract: public Base
{
private:
	State();
	virtual ~State() = default;

public:
	virtual HRESULT Enter_State() = 0;
	virtual HRESULT Update_State() = 0;
	virtual HRESULT Exit_State() = 0;


public:
	virtual void Free() override;
};

END