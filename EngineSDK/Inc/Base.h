#pragma once

/* 게임 내에 사용되는 대다수 클래스의 부모 클래스다. */
/* 레퍼런스 관리의 기능을 제공해준다. */
#include "Engine_Defines.h"

BEGIN(Engine)

class ENGINE_DLL Base abstract
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

END