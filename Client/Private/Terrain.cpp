#include "Terrain.h"
#include "GameInstance.h"

Terrain::Terrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: GameObject{ pDevice, pContext }
{
}

Terrain::Terrain(const Terrain& Prototype)
	: GameObject{ Prototype }
{
}

HRESULT Terrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT Terrain::Initialize(void* pArg)
{
 	GameObject::GAMEOBJECT_DESC	Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Terrain"));
	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = 0.f;


	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;



	return S_OK;
}

void Terrain::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void Terrain::Update(_float fTimeDelta)
{
	int a = 10;
}

void Terrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(Renderer::RENDER_NONBLEND, this);
}

HRESULT Terrain::Render()
{
	if (FAILED(Bind_SR()))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Bind_Input_Assembler()))
		return E_FAIL;

	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;


	//m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT Terrain::Ready_Components()
{
	/* 내가 사용하기좋도록 내 멤버에도 저장을 하고 */
	/* 다른 객체가 검색 할 수 있도록 맵에도 보관한다 . */

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		reinterpret_cast<Component**>(&m_pTextureCom), TEXT("Com_Texture"))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		reinterpret_cast<Component**>(&m_pVIBufferCom), TEXT("Com_VIBuffer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		reinterpret_cast<Component**>(&m_pShaderCom), TEXT("Com_Shader"))))
		return E_FAIL;

	return S_OK;
}

HRESULT Terrain::Bind_SR()
{
	if (FAILED(m_pTransformCom->Bind_SR(m_pShaderCom, "g_WorldMatrix")))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_VP_Transform_ShaderResource(m_pShaderCom, "g_ViewMatrix", PipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_VP_Transform_ShaderResource(m_pShaderCom, "g_ProjMatrix", PipeLine::D3DTS_PROJ)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_SR(m_pShaderCom, "g_DiffuseTexture", 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;

	// 0번째 빛 즉, 햇빛을 가져온 것
	// 추후에 내가 여러 빛을 만들어서 사용한다 가정하였을 때, 제일 0번째는 해가 되기 때문
	const LIGHT_DESC* pLightDesc = m_pGameInstance->Get_LightDesc(0);
	if (nullptr == pLightDesc)
		return E_FAIL;

	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDir", &pLightDesc->vDirection, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightDiffuse", &pLightDesc->vDiffuse, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightAmbient", &pLightDesc->vAmbient, sizeof(_float4)), E_FAIL);
	FAILED_CHECK_RETURN(m_pShaderCom->Bind_RawValue("g_vLightSpecular", &pLightDesc->vSpecular, sizeof(_float4)), E_FAIL);

	//m_pTextureCom->Bind_SR(m_pShaderCom, "g_Texture", 0);

	return S_OK;
}

Terrain* Terrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	Terrain* pInstance = new Terrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

GameObject* Terrain::Clone(void* pArg)
{
	Terrain* pInstance = new Terrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void Terrain::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);


}
