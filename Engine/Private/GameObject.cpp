#include "GameObject.h"
#include "GameInstance.h"

GameObject::GameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice { pDevice }
    , m_pContext { pContext }
    , m_pGameInstance { GameInstance::GetInstance() }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

GameObject::GameObject(const GameObject& Prototype)
    : m_pDevice{ Prototype.m_pDevice }
    , m_pContext{ Prototype.m_pContext }
    , m_pGameInstance{ Prototype.m_pGameInstance }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT GameObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT GameObject::Initialize(void* pArg)
{
    GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);

    // 멤버 변수에 키값을 받아와서 어떤 오브젝트인지 UIObject에 전달
    lstrcpy(m_szGameObjectTag, pDesc->szGameObjectTag);

    return S_OK;
}

void GameObject::Priority_Update(_float fTimeDelta)
{
}

void GameObject::Update(_float fTimeDelta)
{

}

void GameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT GameObject::Render()
{
    return S_OK;
}

HRESULT GameObject::Add_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, Component** ppOut, const _wstring& strComponentTag, void* pArg)
{
    //
    Component* pComponent = dynamic_cast<Component*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT, iLevelIndex, strPrototypeTag, pArg));
    if (nullptr == pComponent)
        return E_FAIL;

    *ppOut = pComponent;

    if (m_mapComponent.end() == m_mapComponent.find(strPrototypeTag))
    {
        m_mapComponent.emplace(strPrototypeTag, pComponent);
        Safe_AddRef(pComponent);
    }

    return S_OK;
}

void GameObject::Free()
{
    __super::Free();

    for (auto& Pair : m_mapComponent)
        Safe_Release(Pair.second);

    m_mapComponent.clear();

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
    Safe_Release(m_pGameInstance);
}
