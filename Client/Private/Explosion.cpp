#include "Explosion.h"
#include "GameInstance.h"


CExplosion::CExplosion(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CGameObject{ pDevice, pContext }
{
}

CExplosion::CExplosion(const CExplosion& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CExplosion::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CExplosion::Initialize(void* pArg)
{
	CGameObject::GAMEOBJECT_DESC	Desc{};

	lstrcpy(Desc.szGameObjectTag, TEXT("GameObject_Explosion"));
	Desc.fSpeedPerSec = 0.f;
	Desc.fRotationPerSec = 0.f;

	if (FAILED(__super::Initialize(&Desc)))
		return E_FAIL;

	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_pTransformCom->Set_State(CTransform::STATE_POS, XMVectorSet(10, 10, 10, 1));

	return S_OK;
}

void CExplosion::Priority_Update(_float fTimeDelta)
{


	int a = 10;
}

void CExplosion::Update(_float fTimeDelta)
{
	m_pVIBufferCom->Spread(fTimeDelta);


}

void CExplosion::Late_Update(_float fTimeDelta)
{


	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_NONBLEND, this);
}

HRESULT CExplosion::Render()
{
	if (FAILED(Bind_ShaderResources()))
		return E_FAIL;

	m_pShaderCom->Begin(0);

	m_pVIBufferCom->Bind_Input_Assembler();

	m_pVIBufferCom->Render();

	return S_OK;
}

HRESULT CExplosion::Ready_Components()
{
	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_VIBuffer_Particle_Explosion"),
		reinterpret_cast<CComponent**>(&m_pVIBufferCom), TEXT("Com_VIBuffer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Shader_VtxPosParticle"),
		reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader"))))
		return E_FAIL;

	/* Com_Texture */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, TEXT("Prototype_Component_Texture_Snow"),
		reinterpret_cast<CComponent**>(&m_pTextureCom), TEXT("Com_Texture"))))
		return E_FAIL;



	return S_OK;
}

HRESULT CExplosion::Bind_ShaderResources()
{
	if (FAILED(m_pTransformCom->Bind_SR("g_WorldMatrix", m_pShaderCom)))
		return E_FAIL;

	if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ViewMatrix", m_pShaderCom, CPipeLine::D3DTS_VIEW)))
		return E_FAIL;
	if (FAILED(m_pGameInstance->Bind_VP_Transform_SR("g_ProjMatrix", m_pShaderCom, CPipeLine::D3DTS_PROJ)))
		return E_FAIL;

	if (FAILED(m_pTextureCom->Bind_SR("g_DiffuseTexture", m_pShaderCom, 0)))
		return E_FAIL;

	if (FAILED(m_pShaderCom->Bind_RawValue("g_vCamPosition", m_pGameInstance->Get_CamPosition(), sizeof(_float4))))
		return E_FAIL;



	return S_OK;
}


CExplosion* CExplosion::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CExplosion* pInstance = new CExplosion(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Created : CExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CExplosion::Clone(void* pArg)
{
	CExplosion* pInstance = new CExplosion(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : CExplosion");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CExplosion::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom);
	Safe_Release(m_pShaderCom);
	Safe_Release(m_pVIBufferCom);


}
