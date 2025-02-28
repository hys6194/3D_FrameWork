#include "ContainerObject.h"
#include "PartObject.h"
#include "GameInstance.h"

ContainerObject::ContainerObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    :GameObject{ pDevice, pContext }
{
}

ContainerObject::ContainerObject(const ContainerObject& Prototype)
    : GameObject{ Prototype }
{

}

HRESULT ContainerObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT ContainerObject::Initialize(void* pArg)
{
    CONTAINEROBJ_DESC* pDesc = static_cast<CONTAINEROBJ_DESC*>(pArg);

    m_iNumPartObjects = pDesc->iNumPartObjects;
    m_vecParts.resize(m_iNumPartObjects);

    FAILED_CHECK_RETURN(__super::Initialize(pDesc), E_FAIL);

    return S_OK;
}

void ContainerObject::Priority_Update(_float fTimeDelta)
{
    for (auto& iter : m_vecParts)
    {
        if (nullptr != iter)
            iter->Priority_Update(fTimeDelta);
    }
}

void ContainerObject::Update(_float fTimeDelta)
{
    for (auto& iter : m_vecParts)
    {
        if (nullptr != iter)
            iter->Update(fTimeDelta);
    }
}

void ContainerObject::Late_Update(_float fTimeDelta)
{
    for (auto& iter : m_vecParts)
    {
        if (nullptr != iter)
            iter->Late_Update(fTimeDelta);
    }
}

HRESULT ContainerObject::Render()
{
    return S_OK;
}

HRESULT ContainerObject::Add_PartObject(_uint iPrototypeLevel, const wstring& strPrototypeTag, _uint iPartIndex, void* pArg)
{
    //// 원형 객체를 사본객체로 생성
    //CBase* pBase = m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevel, strPrototypeTag, pArg);
    //
    //// 생성한 사본 객체를 받아온 파츠의 부위를 체크하여 해당 배열에 넣는다
    //m_vecParts[iPartIndex] = dynamic_cast<PartObject*>(pBase);
    PartObject* pPartObject = dynamic_cast<PartObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_GAMEOBJECT, iPrototypeLevel, strPrototypeTag, pArg));
    NULL_CHECK_RETURN(pPartObject, E_FAIL);
    m_vecParts[iPartIndex] = pPartObject;

    return S_OK;
}

void ContainerObject::Free()
{
    __super::Free();

    for (auto& iter : m_vecParts)
        Safe_Release(iter);

    m_vecParts.clear();
}
