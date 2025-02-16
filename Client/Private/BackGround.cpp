#include "BackGround.h"

#include "GameInstance.h"

BackGround::BackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: UIObject{ pDevice, pContext }
{
}

BackGround::BackGround(const BackGround& Prototype)
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

	if (FAILED(Ready_Component()))
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
	m_pGameInstance->Add_RenderObject(Renderer::RENDER_PRIORITY, this);
}

HRESULT BackGround::Render()
{
	_float4x4			f4Matrix;

	// 항등행렬로 만들기
 	XMStoreFloat4x4(&f4Matrix, XMMatrixIdentity());

	m_pShaderCom->Apply_Matirx("g_WorldMatrix", &f4Matrix);
	m_pShaderCom->Apply_Matirx("g_ViewMatrix", &f4Matrix);
	m_pShaderCom->Apply_Matirx("g_ProjMatrix", &f4Matrix);

	m_pTextureCom->Apply_SR(m_pShaderCom, "g_Texture", 0);

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Render();


    return S_OK;
}

HRESULT BackGround::Ready_Component()
{
	//if (FAILED(__super::Add_Component(LEVEL_LOGO, TEXT("Prototype_Component_Texture_BackGround"),
	//	reinterpret_cast<Component**>(&m_pTextureCom), TEXT("Com_Texture"))))
	//	return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_MENU, TEXT("Prototype_Component_Texture_BackGround"),
		reinterpret_cast<Component**>(&m_pTextureCom), TEXT("Com_Texture"))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_MENU, TEXT("Prototype_Component_VIBuffer_Rect"),
		reinterpret_cast<Component**>(&m_pVIBufferCom), TEXT("Com_VIBuffer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_MENU, TEXT("Prototype_Component_Shader_VtxPosTex"),
		reinterpret_cast<Component**>(&m_pShaderCom), TEXT("Com_Shader"))))
		return E_FAIL;

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

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
