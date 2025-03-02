#include "UIObject.h"

UIObject::UIObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
	:GameObject { pDevice, pContext }
{
}

UIObject::UIObject(const UIObject& Prototype)
	: GameObject { Prototype }
{
}

HRESULT UIObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT UIObject::Initialize(void* pArg)
{
	// UI 오브젝트에서 필요한 값을 인자값으로 받아온 pArg를 UIOBJECT_DESC* 타입으로 정적 캐스팅을 해서 사용
	// ____super::Initialize(pArg)에서 Add_Prototype하면서 키값으로 받아온 원본객체의 이름을 저장
	// 그 후, Object_Manager에서 Clone하면서 Initialize에서 만든 UIDesc을 pArg로 받아오고 그 값을 
	// __super::Initialize(pArg)로 전달해서 m_szGameObjectTag에 담아서 알고 있는 구조이다

	if (nullptr != pArg)
	{
		UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

		m_fX = pDesc->fX;
		m_fY = pDesc->fY;
		m_fSizeX = pDesc->fSizeX;
		m_fSizeY = pDesc->fSizeY;
	}

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

	D3D11_VIEWPORT			ViewportDesc{};
	_uint					iNumViewports = { 1 };

	m_pContext->RSGetViewports(&iNumViewports, &ViewportDesc);

	m_pTransformCom->SetUp_Scaled(m_fSizeX, m_fSizeY, 1.f);
	m_pTransformCom->Set_State(Transform::STATE_POS,
		XMVectorSet(m_fX - ViewportDesc.Width * 0.5f, -m_fY + ViewportDesc.Height * 0.5f, 0.f, 1.f));

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
	XMStoreFloat4x4(&m_ProjMatrix, XMMatrixOrthographicLH(ViewportDesc.Width, ViewportDesc.Height, 0.f, 1.f));

	return S_OK;
}

void UIObject::Priority_Update(_float fTimeDelta)
{
}

void UIObject::Update(_float fTimeDelta)
{
}

void UIObject::Late_Update(_float fTimeDelta)
{
}

HRESULT UIObject::Render()
{
	return S_OK;
}

void UIObject::Free()
{
	__super::Free();
}
