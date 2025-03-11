#include "GameObject.h"
#include "GameInstance.h"

CGameObject::CGameObject(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice { pDevice }
    , m_pContext { pContext }
    , m_pGameInstance { CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

CGameObject::CGameObject(const CGameObject& Prototype)
    : m_pDevice{ Prototype.m_pDevice }
    , m_pContext{ Prototype.m_pContext }
    , m_pGameInstance{ Prototype.m_pGameInstance }
   /* , m_pTransformCom{ Prototype.m_pTransformCom }*/
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
    Safe_AddRef(m_pGameInstance);
}

HRESULT CGameObject::Initialize_Prototype()
{
    return S_OK;
}

HRESULT CGameObject::Initialize(void* pArg)
{
    if (nullptr != pArg)
    {
        GAMEOBJECT_DESC* pDesc = static_cast<GAMEOBJECT_DESC*>(pArg);
        lstrcpy(m_szGameObjectTag, pDesc->szGameObjectTag);
    }
   // m_pTransformCom = Transform::Create(m_pDevice, m_pContext);
   // if (nullptr == m_pTransformCom)
   //     return E_FAIL;
   // if (FAILED(m_pTransformCom->Initialize(pArg)))
   //     return E_FAIL;

    HRESULT hr;
    hr = Set_TransformCom(pArg);

    return hr;
}

void CGameObject::Priority_Update(_float fTimeDelta)
{
}

void CGameObject::Update(_float fTimeDelta)
{

}

void CGameObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CGameObject::Render()
{
    return S_OK;
}

HRESULT CGameObject::Add_Component(_uint iLevelIndex, const _wstring& strPrototypeTag, Component** ppOut, const _wstring& strComponentTag, void* pArg)
{
    // Component 사본객체 생성
    Component* pComponent = dynamic_cast<Component*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::TYPE_COMPONENT, iLevelIndex, strPrototypeTag, pArg));
    if (nullptr == pComponent)
        return E_FAIL;

    *ppOut = pComponent;

    if (m_mapComponent.end() == m_mapComponent.find(strComponentTag))
    {
        m_mapComponent.emplace(strComponentTag, pComponent);
        Safe_AddRef(pComponent);
    }

    return S_OK;
}

HRESULT CGameObject::Set_TransformCom(void* pArg)
{
    m_pTransformCom = Transform::Create(m_pDevice, m_pContext);
    if (nullptr == m_pTransformCom)
        return E_FAIL;

    if (FAILED(m_pTransformCom->Initialize(pArg)))
        return E_FAIL;

    // 멤버 변수에 키값을 받아와서 어떤 오브젝트인지 UIObject에 전달
    m_mapComponent.emplace(g_strTransformTag, m_pTransformCom);

    Safe_AddRef(m_pTransformCom);

    return S_OK;
}

void CGameObject::Free()
{
    __super::Free();

    for (auto& Pair : m_mapComponent)
        Safe_Release(Pair.second);

    m_mapComponent.clear();   

    Safe_Release(m_pTransformCom);

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
    Safe_Release(m_pGameInstance);
}
