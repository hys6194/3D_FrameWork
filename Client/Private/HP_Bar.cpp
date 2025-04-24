#include "HP_Bar.h"

#include "GameInstance.h"

CHP_Bar::CHP_Bar(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	: CUIObject{ pDevice, pContext }
{
}

CHP_Bar::CHP_Bar(const CHP_Bar& Prototype)
	:CUIObject{ Prototype }
	, m_pShaderCom{ Prototype.m_pShaderCom }
	, m_pTextureCom{ Prototype.m_pTextureCom }
	, m_pVIBufferCom{ Prototype.m_pVIBufferCom }
{
	Safe_AddRef(m_pShaderCom);
	Safe_AddRef(m_pTextureCom);
	Safe_AddRef(m_pVIBufferCom);
}

HRESULT CHP_Bar::Initialize_Prototype()
{
	// 두 클래스다 최대체력일 때와 0일때에는 렌더하면 안됨

    return S_OK;
}

HRESULT CHP_Bar::Initialize(void* pArg)
{
	// 이 클래스에서 상속받은 Desc의 정보를 여기서 선언해서 사용하는게 맞지만,
	// Create, Add할 때 어떤 클래스를 만들 것인지 알고 있어서
	// -> GameObjectDesc의 멤버인 szGameObjectTag로 어떤 오브젝트를 만들 것인지 설정할 것
	// 어디에서? -> Loader클래스에서
	// 부모 클래스인 UIObject에서 구조체 값을 채워갈 예정

	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_pOwner = pDesc->pOwner;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	if (FAILED(Ready_Component()))
		return E_FAIL;

	return S_OK;
}

void CHP_Bar::Priority_Update(_float fTimeDelta)
{
	int a = 10;
}

void CHP_Bar::Update(_float fTimeDelta)
{
}

void CHP_Bar::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderObject(CRenderer::RENDER_UI, this);
}

HRESULT CHP_Bar::Render()
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

HRESULT CHP_Bar::Ready_Component()
{
	// 다른 객체가 검색할 수 있도록 맵에 보관한다

	if (FAILED(__super::Add_Component(LEVEL_GAMEPLAY, PRO_TEX_HP_BAR,
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

HRESULT CHP_Bar::Bind_SR()
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

CHP_Bar* CHP_Bar::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	CHP_Bar* pInstance = new CHP_Bar(pDevice, pContext);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed To Cloned : BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CHP_Bar::Clone(void* pArg)
{
	CHP_Bar* pInstance = new CHP_Bar(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed To Cloned : BackGround");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CHP_Bar::Free()
{
	__super::Free();

	Safe_Release(m_pShaderCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);

}
