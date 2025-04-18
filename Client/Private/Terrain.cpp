#include "Terrain.h"
#include "GameInstance.h"

CTerrain::CTerrain(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CTerrain::CTerrain(const CTerrain& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CTerrain::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CTerrain::Initialize(void* pArg)
{
 	CGameObject::GAMEOBJECT_DESC	Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Terrain"));
	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = 0.f;


	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;



	return S_OK;
}

void CTerrain::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CTerrain::Update(_float fTimeDelta)
{
	int a = 10;
}

void CTerrain::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CTerrain::Render()
{
	if (FAILED(Bind_SR()))
		return E_FAIL;
	
	if (FAILED(m_pShaderCom->Begin(0)))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Bind_Input_Assembler()))
		return E_FAIL;
	
	if (FAILED(m_pVIBufferCom->Render()))
		return E_FAIL;

#ifdef _DEBUG
	m_pNavigationCom->Render();
#endif

	return S_OK;
}

HRESULT CTerrain::Ready_Components()
{
	/* 내가 사용하기좋도록 내 멤버에도 저장을 하고 */
	/* 다른 객체가 검색 할 수 있도록 맵에도 보관한다 . */

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Terrain"),
		reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_DIFFUSE]), TEXT("Com_Texture"))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Mask"),
		reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_MASK]), TEXT("Com_Texture_Mask"))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Brush"),
		reinterpret_cast<CComponent**>(&m_pTextureCom[TYPE_BRUSH]), TEXT("Com_Texture_Brush"))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Terrain"),
		reinterpret_cast<CComponent**>(&m_pVIBufferCom), TEXT("Com_VIBuffer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxNorTex"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader"))))
		return E_FAIL;

	CNavigation::NAVIGATION_DESC		NaviDesc{};
	NaviDesc.pWorldMatrix				= m_pTransformCom->Get_WorldMatrix_Ptr();

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Navigation"),
		reinterpret_cast<CComponent**>(&m_pNavigationCom), TEXT("Com_Navigation"), &NaviDesc)))
		return E_FAIL;

	return S_OK;
}

HRESULT CTerrain::Bind_SR()
{
	if (FAILED(m_pTransformCom->Bind_SR("g_WorldMatrix", m_pShaderCom)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom,  CPipeLine::D3DTS_VIEW)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom,  CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Bind_SR("g_DiffuseTexture", m_pShaderCom, 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Bind_SR("g_MaskTexture", m_pShaderCom, 0)))
		return E_FAIL;

	if (FAILED(m_pTextureCom[TYPE_DIFFUSE]->Bind_SR("g_BrushTexture", m_pShaderCom, 0)))
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

CTerrain* CTerrain::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CTerrain* pInstance = new CTerrain(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrain::Clone(void* pArg)
{
	CTerrain* pInstance = new CTerrain(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : Terrain");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrain::Free()
{
	__super::Free();

	Safe_Release(m_pNavigationCom);
	Safe_Release(m_pShaderCom);

	for (auto& pTextureCom : m_pTextureCom)
		Safe_Release(pTextureCom);

	Safe_Release(m_pVIBufferCom);


}
