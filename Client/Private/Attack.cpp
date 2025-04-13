#include "Attack.h"
#include "Component.h"

CAttack::CAttack(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :CComponent { pDevice , pContext }
{
}

CAttack::CAttack(const CAttack& Prototype)
    : CComponent{ Prototype }
{
}

HRESULT CAttack::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CAttack::Initialize(void* pArg)
{
    return S_OK;
}

CAttack* CAttack::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CAttack* pInstance = new CAttack(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX("Failed To Cloned : Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

CComponent* CAttack::Clone(void* pArg)
{
    CComponent* pInstance = new CAttack(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Attack");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CAttack::Free()
{
    __super::Free();
}
