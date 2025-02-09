#include "Renderer.h"

#include "GameObject.h"

Renderer::Renderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice { pDevice }
    , m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT Renderer::Initialize()
{
    // 추후 후처리를 여기에서 작업할 예정


    return S_OK;
}

HRESULT Renderer::Add_RenderObject(RENDERERGROUP eRenderGroup, GameObject* pRenderObject)
{
    if (eRenderGroup >= RENDER_END ||
        nullptr == pRenderObject)
        return E_FAIL;

    m_listRenderer[eRenderGroup].push_back(pRenderObject);

    Safe_AddRef(pRenderObject);

    return S_OK;
}

void Renderer::Draw()
{
    if (FAILED(Render_Priority()))
        return;

    if (FAILED(Render_NonBlend()))
        return;

    if (FAILED(Render_Blend()))
        return;

    if (FAILED(Render_UI()))
        return;
}

void Renderer::Clear()
{
    for (size_t i = 0; i < RENDER_END; ++i)
    {
        for (auto& iter : m_listRenderer[i])
        {
            Safe_Release(iter);
        }

        m_listRenderer[i].clear();
    }

    
}

HRESULT Renderer::Render_Priority()
{
    for (auto& iter : m_listRenderer[RENDER_PRIORITY])
    {
        if (nullptr != iter)
            iter->Render();

        Safe_Release(iter);
    }
    m_listRenderer[RENDER_PRIORITY].clear();

    return S_OK;
}

HRESULT Renderer::Render_NonBlend()
{
    for (auto& iter : m_listRenderer[RENDER_NONBLEND])
    {
        if (nullptr != iter)
            iter->Render();

        Safe_Release(iter);
    }
    m_listRenderer[RENDER_NONBLEND].clear();

    return S_OK;
}

HRESULT Renderer::Render_Blend()
{
    for (auto& iter : m_listRenderer[RENDER_BLEND])
    {
        if (nullptr != iter)
            iter->Render();

        Safe_Release(iter);
    }
    m_listRenderer[RENDER_BLEND].clear();

    return S_OK;
}

HRESULT Renderer::Render_UI()
{
    for (auto& iter : m_listRenderer[RENDER_UI])
    {
        if (nullptr != iter)
            iter->Render();

        Safe_Release(iter);
    }
    m_listRenderer[RENDER_UI].clear();

    return S_OK;
}

Renderer* Renderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    Renderer* pInstance = new Renderer(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : Renderer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Renderer::Free()
{
    __super::Free();


    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
