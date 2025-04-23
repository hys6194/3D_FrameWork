#include "HP_Frame.h"

#include "GameInstance.h"

CHP_Frame::CHP_Frame(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CHP_Frame::CHP_Frame(const CHP_Frame& Prototype)
	:CUIObject{ Prototype }
	, m_pShaderCom { Prototype.m_pShaderCom}
	, m_pTextureCom{ Prototype.m_pTextureCom }
	, m_pVIBufferCom { Prototype.m_pVIBufferCom }
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pVIBufferCom);
}

HRESULT CHP_Frame::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CHP_Frame::Initialize(void* pArg)
{
	HPFRAME_DESC* pDesc = static_cast<HPFRAME_DESC*>(pArg);

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component(pDesc->strFrameTag)))
		return E_FAIL;

	return S_OK;
}

void CHP_Frame::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CHP_Frame::Update(_float fTimeDelta)
{
}

void CHP_Frame::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CHP_Frame::Render()
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

HRESULT CHP_Frame::Ready_Component(const _wstring& strTag)
{
	// 다른 객체가 검색할 수 있도록 맵에 보관한다
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, strTag,
		reinterpret_cast<CComponent**>(&m_pTextureCom), TEXT("Com_Texture"))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_COM_VI_RECT,
		reinterpret_cast<CComponent**>(&m_pVIBufferCom), TEXT("Com_VIBuffer"))))
		return E_FAIL;

	/* Com_Shader */
	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_SHADER_POS,
		reinterpret_cast<CComponent**>(&m_pShaderCom), TEXT("Com_Shader"))))
		return E_FAIL;

	return S_OK;
}

HRESULT CHP_Frame::Bind_SR()
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

CHP_Frame* CHP_Frame::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHP_Frame* pInstance = new CHP_Frame(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Cloned : BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHP_Frame::Clone(void* pArg)
{
	CHP_Frame* pInstance = new CHP_Frame(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP_Frame::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
