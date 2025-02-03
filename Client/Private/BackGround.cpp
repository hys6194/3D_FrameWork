#include "BackGround.h"

BackGround::BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: UIObject{ pDevice, pContext }
{
}

BackGround::BackGround(BackGround& Prototype)
	:UIObject{ Prototype }
{
}

HRESULT BackGround::Initialize_Prototype()
{
    return S_OK;
}

HRESULT BackGround::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    return S_OK;
}

void BackGround::Priority_Update(_float fTimeDelta)
{
}

void BackGround::Update(_float fTimeDelta)
{
}

void BackGround::Late_Update(_float fTimeDelta)
{
}

HRESULT BackGround::Render()
{
    return S_OK;
}

BackGround* BackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

GameObject* BackGround::Clone(void* pArg)
{
	BackGround* pInstance = new BackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void BackGround::Free()
{
}
