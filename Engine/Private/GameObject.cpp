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

void GameObject::Free()
{
    __super::Free();

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
    Safe_Release(m_pGameInstance);
}
