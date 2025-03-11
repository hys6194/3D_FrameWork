#include "PartObject.h"

PartObject::PartObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CGameObject{ pDevice, pContext }
{
}

PartObject::PartObject(const PartObject& Prototype)
    : CGameObject{ Prototype }
{

}

HRESULT PartObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT PartObject::Initialize(void* pArg)
{
    PARTOBJ_DESC* pDesc = static_cast<PARTOBJ_DESC*>(pArg);
    m_pParentMatrix = pDesc->pParentMatrix;

    FAILED_CHECK_RETURN(__super::Initialize(pArg), E_FAIL);

    return S_OK;
}


void PartObject::Priority_Update(_float fTimeDelta)
{
}

void PartObject::Update(_float fTimeDelta)
{
}

void PartObject::Late_Update(_float fTimeDelta)
{
}

HRESULT PartObject::Render()
{
    return S_OK;
}

void PartObject::Free()
{
    __super::Free();
}
