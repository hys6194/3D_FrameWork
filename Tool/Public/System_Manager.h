#pragma once

#include "pch.h"
#include "Base.h"

class System_Manager : public Base
{
private:
	DECLARE_SINGLETON(System_Manager);

public:
	static System_Manager* Create();
	virtual void Free() override;

};

