#include "Renderer.h"

#include "GameObject.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice { pDevice }
    , m_pContext{ pContext }
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
    // 추후 후처리를 여기에서 작업할 예정


    return S_OK;
}

HRESULT CRenderer::Add_RenderObject(RENDERERGROUP eRenderGroup, CGameObject* pRenderObject)
{
    if (eRenderGroup >= RENDER_END ||
        nullptr == pRenderObject)
        return E_FAIL;

    m_listRenderer[eRenderGroup].push_back(pRenderObject);

    Safe_AddRef(pRenderObject);

    return S_OK;
}

void CRenderer::Draw()
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

void CRenderer::Clear()
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

HRESULT CRenderer::Render_Priority()
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

HRESULT CRenderer::Render_NonBlend()
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

HRESULT CRenderer::Render_Blend()
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

HRESULT CRenderer::Render_UI()
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

CRenderer* CRenderer::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    CRenderer* pInstance = new CRenderer(pDevice, pContext);

    if (FAILED(pInstance->Initialize()))
    {
        MSG_BOX("Failed to Created : Renderer");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CRenderer::Free()
{
    __super::Free();

    Clear();

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
}
