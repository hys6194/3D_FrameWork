#include "BackGround.h"

#include "GameInstance.h"

CBackGround::CBackGround(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: UIObject{ pDevice, pContext }
{
}

CBackGround::CBackGround(const CBackGround& Prototype)
	:UIObject{ Prototype }
	, m_eLevel{ Prototype.m_eLevel}
{
}

HRESULT CBackGround::Initialize_Prototype(LEVEL eLevel)
{
	m_eLevel = eLevel;

    return S_OK;
}

HRESULT CBackGround::Initialize(void* pArg)
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

void CBackGround::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CBackGround::Update(_float fTimeDelta)
{
}

void CBackGround::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(Renderer::RENDER_PRIORITY, this);
}

HRESULT CBackGround::Render()
{
	//_float4x4			f4Matrix;
	//
	//// 항등행렬로 만들기
 	//XMStoreFloat4x4(&f4Matrix, XMMatrixIdentity());
	//
	//m_pShaderCom->Bind_Matrix(&f4Matrix, "g_WorldMatrix");
	//m_pShaderCom->Bind_Matrix(&f4Matrix, "g_ViewMatrix");
	//m_pShaderCom->Bind_Matrix(&f4Matrix, "g_ProjMatrix");
	//
  	//m_pTextureCom->Bind_SR(m_pShaderCom, "g_Texture", 0);

	if (FAILED(Bind_SR()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Input_Assembler()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


    return S_OK;
}

HRESULT CBackGround::Ready_Component()
{
	// 다른 객체가 검색할 수 있도록 맵에 보관한다

	switch (m_eLevel)
	{
		case LEVEL_LOGO :
			//if (FAILED(__super::Add_Component(LEVEL_LOGO, PRO_TEX_BACKGROUND,
			//	reinterpret_cast<Component**>(&m_pTextureCom), TEXT("Com_Texture"))))
			//	return E_FAIL;

			if (FAILED(__super::Add_Component(LEVEL_LOGO, PRO_TEX_LOGO1,
				reinterpret_cast<Component**>(&m_pTextureCom), TEXT("Com_Texture"))))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LEVEL_LOGO, PRO_COM_VI_RECT,
				reinterpret_cast<Component**>(&m_pVIBufferCom), TEXT("Com_VIBuffer"))))
				return E_FAIL;

			/* Com_Shader */
			if (FAILED(__super::Add_Component(LEVEL_LOGO, PRO_SHADER_POS,
				reinterpret_cast<Component**>(&m_pShaderCom), TEXT("Com_Shader"))))
				return E_FAIL;



			break;
		case LEVEL_MENU :
			if (FAILED(__super::Add_Component(LEVEL_MENU, PRO_TEX_BACKGROUND,
				reinterpret_cast<Component**>(&m_pTextureCom), TEXT("Com_Texture"))))
				return E_FAIL;

			if (FAILED(__super::Add_Component(LEVEL_MENU, PRO_COM_VI_RECT,
				reinterpret_cast<Component**>(&m_pVIBufferCom), TEXT("Com_VIBuffer"))))
				return E_FAIL;

			/* Com_Shader */
			if (FAILED(__super::Add_Component(LEVEL_MENU, PRO_SHADER_POS,
				reinterpret_cast<Component**>(&m_pShaderCom), TEXT("Com_Shader"))))
				return E_FAIL;

			break;

		default:
			break;
	}

	return S_OK;
}

HRESULT CBackGround::Bind_SR()
{
	if (FAILED(m_pTransformCom->Bind_SR("g_WorldMatrix", m_pShaderCom)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ViewMatrix", &m_ViewMatrix)))
		return E_FAIL;
	if (FAILED(m_pShaderCom->Bind_Matrix("g_ProjMatrix", &m_ProjMatrix)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_SR("g_Texture", m_pShaderCom, 0)))
		return E_FAIL; 
	
	return S_OK;

}

CBackGround* CBackGround::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, LEVEL eLevel)
{
	CBackGround* pInstance = new CBackGround(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype(eLevel)))
	{
		MSG_BOX("Failed To Cloned : BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

GameObject* CBackGround::Clone(void* pArg)
{
	CBackGround* pInstance = new CBackGround(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CBackGround::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
