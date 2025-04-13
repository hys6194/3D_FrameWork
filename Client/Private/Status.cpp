#include "Status.h"

CStatus::CStatus(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : CComponent{ pDevice ,pContext }
{
}

CStatus::CStatus(const CStatus& Prototype)
    : CComponent { Prototype }
{
}

HRESULT CStatus::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CStatus::Initialize(void* pArg)
{
    STATUS_DESC* pDesc = static_cast<STATUS_DESC*>(pArg);

    // 현명. 사랑해요 멤카피
    memcpy(&m_tDesc, pDesc, sizeof(STATUS_DESC));
    memcpy(&m_tMaxDesc, pDesc, sizeof(STATUS_DESC));

    m_pOwner = pDesc->pOwner;

	return S_OK;
}

void CStatus::Take_Damage(_int iDamage)
{
    m_tDesc.iHP = Clamp(m_tDesc.iHP - iDamage, 0, m_tMaxDesc.iHP);

    if (iDamage > 0)
        m_bIsDamaged = true;
}

void CStatus::Calculate_StatusInfo(_int iAttack)
{
    // 여기에서 피격처리 하는 것이 좋아보이긴 함
}

CStatus* CStatus::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CStatus* pInstance = new CStatus(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Cloned : Status");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CStatus::Clone(void* pArg)
{
    CComponent* pInstance = new CStatus(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Status");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CStatus::Free()
{
    __super::Free();
}
