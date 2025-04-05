#pragma once

#include "Base.h"

BEGIN(Engine)

class CCollisionManager : public CBase
{
private:
	CCollisionManager();
	virtual ~CCollisionManager() = default;

public:


public:
	static CCollisionManager* Create();
	virtual void Free() override;
};

END