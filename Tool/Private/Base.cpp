#include "pch.h"
#include "Base.h"

Base::Base()
{
}

_uint Base::AddRef()
{
	return ++m_iRefCnt;
}

_uint Base::Release()
{
	if (0 == m_iRefCnt)
	{
		Free();

		delete this;

		return 0;
	}
	else
		return m_iRefCnt--;
}

void Base::Free()
{
}
