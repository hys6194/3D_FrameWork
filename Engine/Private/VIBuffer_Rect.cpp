#include "VIBuffer_Rect.h"

VIBuffer_Rect::VIBuffer_Rect(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: VIBuffer{ pDevice, pContext }
{
}

VIBuffer_Rect::VIBuffer_Rect(const VIBuffer_Rect& Prototype)
	: VIBuffer{ Prototype }
{
}

HRESULT VIBuffer_Rect::Initialize_Prototype()
{
	return S_OK;
}

HRESULT VIBuffer_Rect::Initialize(void* pArg)
{
	return S_OK;
}

VIBuffer_Rect* VIBuffer_Rect::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	VIBuffer_Rect* pInstance = new VIBuffer_Rect(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : VIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

Component* VIBuffer_Rect::Clone(void* pArg)
{
	Component* pInstance = new VIBuffer_Rect(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : VIBuffer_Rect");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void VIBuffer_Rect::Free()
{
	__super::Free();
}
