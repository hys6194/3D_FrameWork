#include "BackGround.h"

#include "GameInstance.h"

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
	// 이 클래스에서 상속받은 Desc의 정보를 여기서 선언해서 사용하는게 맞지만,
	// Create, Add할 때 어떤 클래스를 만들 것인지 알고 있어서
	// -> GameObjectDesc의 멤버인 szGameObjectTag로 어떤 오브젝트를 만들 것인지 설정할 것
	// 어디에서? -> Loader클래스에서
	// 부모 클래스인 UIObject에서 구조체 값을 채워갈 예정


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

    return S_OK;
}

void BackGround::Priority_Update(_float fTimeDelta)
{
	int a = 10;
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

HRESULT BackGround::Ready_Component()
{
	

	return S_OK;
}

BackGround* BackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	BackGround* pInstance = new BackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Cloned : BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
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
	__super::Free();
}
