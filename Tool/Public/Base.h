#pragma once

#include "Engine_Defines.h"

class Base
{
protected:
	Base();
	virtual ~Base() = default;

public:
	/* 레퍼런스 카운트를 증가시키낟. */
	_uint AddRef();

	/* 레퍼런스 카운트를 감소시킨다. or 삭제한다. */
	_uint Release();

private:
	_uint			m_iRefCnt = { 0 };

public:
	virtual void Free();

};

