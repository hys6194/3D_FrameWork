#include "Sky.h"
#include "GameInstance.h"

Sky::Sky(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: GameObject{ pDevice, pContext }
{
}

Sky::Sky(const Sky& Prototype)
	: GameObject{ Prototype }
{
}

HRESULT Sky::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Sky::Initialize(void* pArg)
{
 	GameObject::GAMEOBJECT_DESC	Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Sky"));
	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = 0.f;


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;



	return S_OK;
}

void Sky::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void Sky::Update(_float fTimeDelta)
{
	int a = 10;
}

void Sky::Late_Update(_float fTimeDelta)
{
	// 스카이 박스는 기초적으로 카메라를 감싸는 형태로 존재함
	m_pTransformCom->Set_State(Transform::STATE_POS,
		XMLoadFloat4(m_pGameInstance->Get_CamPosition()));

	m_pGameInstance->Add_RenderObject(Renderer::RENDER_NONBLEND, this);
}

HRESULT Sky::Render()
{
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

HRESULT Sky::Ready_Components()
{
	/* 내가 사용하기좋도록 내 멤버에도 저장을 하고 */
	/* 다른 객체가 검색 할 수 있도록 맵에도 보관한다 . */

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_TEX_SKY,
		reinterpret_cast<Component**>(&m_pTextureCom), TEXT("Com_Texture"))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_VI_CUBE,
		reinterpret_cast<Component**>(&m_pVIBufferCom), TEXT("Com_VIBuffer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_CUBE,
		reinterpret_cast<Component**>(&m_pShaderCom), TEXT("Com_Shader"))))
		return E_FAIL;

	return S_OK;
}

HRESULT Sky::Bind_SR()
{
	FAILED_CHECK_RETURN(m_pTransformCom->Bind_SR("g_WorldMatrix", m_pShaderCom), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, PipeLine::D3DTS_VIEW), E_FAIL);
	FAILED_CHECK_RETURN(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, PipeLine::D3DTS_PROJ), E_FAIL);
	FAILED_CHECK_RETURN(m_pTextureCom->Bind_SR("g_Texture", m_pShaderCom, 2),E_FAIL);

	return S_OK;
}

Sky* Sky::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Sky* pInstance = new Sky(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Sky");
		Safe_Release(pInstance);
	}

	return pInstance;
}

GameObject* Sky::Clone(void* pArg)
{
	Sky* pInstance = new Sky(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Sky");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Sky::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);


}
