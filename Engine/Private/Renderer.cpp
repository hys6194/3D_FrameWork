#include "Renderer.h"

#include "GameObject.h"
#include "GameInstance.h"

CRenderer::CRenderer(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice { pDevice }
    , m_pContext{ pContext }
    , m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CRenderer::Initialize()
{
    // 추후 후처리를 여기에서 작업할 예정
    _uint iNumViewPorts = 1;
    D3D11_VIEWPORT ViewPortsDesc = {};

    m_pContext->RSGetViewports(&iNumViewPorts, &ViewPortsDesc);

    FAILED_CHECK_RETURN(m_pGameInstance->Add_RenderTarget(TARGET_DIFF, ViewPortsDesc.Width, ViewPortsDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, _float4(1.f, 1.f, 1.f, 1.f)), E_FAIL);

    // 노말은 8bit로 저장하게 되면 소수 정밀도가 떨어지게 됨 그래서 제대로 된 노말 표현이 안됨
    FAILED_CHECK_RETURN(m_pGameInstance->Add_RenderTarget(TARGET_NORM, ViewPortsDesc.Width, ViewPortsDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f)), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Add_RenderTarget(TARGET_SHAD, ViewPortsDesc.Width, ViewPortsDesc.Height, DXGI_FORMAT_R16G16B16A16_UNORM, _float4(1.f, 1.f, 1.f, 1.f)), E_FAIL);

    FAILED_CHECK_RETURN(m_pGameInstance->Add_MRT(MRT_GAMEOBJ, TARGET_DIFF), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Add_MRT(MRT_GAMEOBJ, TARGET_NORM), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Add_MRT(MRT_LIGHT, TARGET_SHAD), E_FAIL);

    // 셰이더에 던지기 위한 행렬 생성
    // 월드 행렬에 정규화된 깊이값인 1을 넣어서 원근 투영할 수 있는 행렬을 생성
    XMStoreFloat4x4(&m_matWorld, XMMatrixScaling(ViewPortsDesc.Width, ViewPortsDesc.Height, 1.f));
    XMStoreFloat4x4(&m_matView, XMMatrixIdentity());
    XMStoreFloat4x4(&m_matProj, XMMatrixOrthographicLH(
        ViewPortsDesc.Width, ViewPortsDesc.Height, 
        0.f, 1.f));

    m_pVIBuffer = CVIBuffer_Rect::Create(m_pDevice, m_pContext);
    NULL_CHECK_RETURN(m_pVIBuffer, E_FAIL);

    m_pShader = CShader::Create(m_pDevice, m_pContext, TEXT("../Bin/ShaderFiles/Shader_Deferred.hlsl"), VTXPOSTEX::ElementDesc, VTXPOSTEX::iNumElements);
    NULL_CHECK_RETURN(m_pShader, E_FAIL);


    FAILED_CHECK_RETURN(m_pGameInstance->Ready_RT_Debug(TARGET_DIFF, 150.0f, 150.0f, 300.f, 300.f), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Ready_RT_Debug(TARGET_NORM, 150.0f, 450.0f, 300.f, 300.f), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Ready_RT_Debug(TARGET_SHAD, 450.0f, 150.0f, 300.f, 300.f), E_FAIL);

    //FAILED_CHECK_RETURN(m_pGameInstance->Ready_RT_Debug(TARGET_DIFF, 50.f,50.f, 100.f, 100.f), E_FAIL);
    //FAILED_CHECK_RETURN(m_pGameInstance->Ready_RT_Debug(TARGET_NORM, 50.f,150.f, 100.f, 100.f), E_FAIL);
    //FAILED_CHECK_RETURN(m_pGameInstance->Ready_RT_Debug(TARGET_SHAD, 50.f,250.f, 100.f, 100.f), E_FAIL);

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

#ifdef _DEBUG
    if (FAILED(Render_Debug()))
        return;
#endif

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

HRESULT CRenderer::Render_NonLight()
{
    for (auto& pRenderObject : m_listRenderer[RENDER_NONLIGHT])
    {
        if (nullptr != pRenderObject)
            pRenderObject->Render();

        Safe_Release(pRenderObject);
    }

    m_listRenderer[RENDER_NONLIGHT].clear();

    return S_OK;
}

HRESULT CRenderer::Render_NonBlend()
{
    FAILED_CHECK_RETURN(m_pGameInstance->Begin_MRT(MRT_GAMEOBJ), E_FAIL);

    for (auto& iter : m_listRenderer[RENDER_NONBLEND])
    {
        if (nullptr != iter)
            iter->Render();

        Safe_Release(iter);
    }
    m_listRenderer[RENDER_NONBLEND].clear();

    FAILED_CHECK_RETURN(m_pGameInstance->End_MRT(), E_FAIL);

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

HRESULT CRenderer::Render_Lights()
{
    FAILED_CHECK_RETURN(m_pGameInstance->Begin_MRT(MRT_LIGHT), E_FAIL);

    FAILED_CHECK_RETURN(m_pGameInstance->Bind_RT_SR(m_pShader, "g_NormalTexture", TARGET_NORM), E_FAIL);

    m_pShader->Bind_Matrix("g_WorldMatrix", &m_matWorld);
    m_pShader->Bind_Matrix("g_ViewMatrix", &m_matView);
    m_pShader->Bind_Matrix("g_ProjMatrix", &m_matProj);


    FAILED_CHECK_RETURN(m_pGameInstance->End_MRT(), E_FAIL);

    return S_OK;
}

HRESULT CRenderer::Render_Deferred()
{
    return S_OK;
}

#ifdef _DEBUG

HRESULT CRenderer::Render_Debug()
{
    FAILED_CHECK_RETURN(m_pShader->Bind_Matrix("g_ViewMatrix", &m_matView), E_FAIL);
    FAILED_CHECK_RETURN(m_pShader->Bind_Matrix("g_ProjMatrix", &m_matProj), E_FAIL);

    FAILED_CHECK_RETURN(m_pGameInstance->Render_RT_Debug(MRT_GAMEOBJ, m_pShader, m_pVIBuffer), E_FAIL);
    FAILED_CHECK_RETURN(m_pGameInstance->Render_RT_Debug(MRT_LIGHT, m_pShader, m_pVIBuffer), E_FAIL);

    return S_OK;
}

#endif

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

    Safe_Release(m_pVIBuffer);
    Safe_Release(m_pShader);

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
}
