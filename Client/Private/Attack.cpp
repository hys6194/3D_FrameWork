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

void CAttack::Update_CoolTime(_float fTimeDelta)
{
    for (auto& Pair : m_mapPattern)
    {
        Pair.second->Update_CoolTime(fTimeDelta);
    }
}

CAttack_Base* CAttack::Find_Attackable()
{
    for (auto& Pair : m_mapPattern)
    {
        if(Pair.second->Check_Attackable())
            return Pair.second;
    }

    return nullptr;
}

CAttack_Base* CAttack::Find_AttackPattern(_uint iStateTag)
{
    auto Pair1 = m_mapPattern.find(iStateTag);

    if(m_mapPattern.end() != Pair1)
    {
        if(Pair1->second->Check_Attackable())
            return Pair1->second;
    }

    return nullptr;
}

CAttack_Base* CAttack::Find_Attackable(_bool bSoruColl, _bool bDestColl)
{
    for (auto& Pair : m_mapPattern)
    {
        if (Pair.second->Check_Attackable())
            return Pair.second;
    }

    return nullptr;
}


void CAttack::Regist_AttackPattern(_uint iStateEnum, CAttack_Base* pState)
{
    if (!pState)
        return;

    m_mapPattern.insert({iStateEnum, pState});
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

    //for (auto& Pair : m_mapPattern)
    //{
    //    Pair.second->Free();
    //}

    m_mapPattern.clear();
}
