#include "State.h"

State::State()
{
}

HRESULT State::Initialize_Prototype()
{
	return S_OK;
}

HRESULT State::Initialize(void* pArg)
{

	return S_OK;
}

Base* State::Clone(void* pArg)
{
	Base* pInstance = new State();

	if (FAILED(Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : State");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void State::Free()
{
	__super::Free();
}
