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
	UIOBJECT_DESC* pDesc = static_cast<UIOBJECT_DESC*>(pArg);

	m_fX = pDesc->fX;
	m_fY = pDesc->fY;
	m_fSizeX = pDesc->fSizeX;
	m_fSizeY = pDesc->fSizeY;

	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;

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
