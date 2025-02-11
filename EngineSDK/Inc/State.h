#pragma once

#include"Base.h"

BEGIN(Engine)

class ENGINE_DLL State : public Base
{
private:
	State();
	virtual ~State() = default;

public:
	HRESULT Initialize_Prototype();
	HRESULT Initialize(void* pArg);
	void Update(_float fTimeDelta);

public:
	virtual Base* Clone(void* pArg);
	virtual void Free() override;
};

END