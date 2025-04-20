#include "Picking.h"

#include "GameInstance.h"

CPicking::CPicking(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice{ pDevice }
    , m_pContext{ pContext }
    , m_pGameInstance{ CGameInstance::GetInstance() }
{
    Safe_AddRef(m_pGameInstance);
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CPicking::Initialize(HWND hWnd)
{
    m_hWnd = hWnd;

    D3D11_VIEWPORT          Viewport{};
    _uint                   iNumViewports = { 1 };
    m_pContext->RSGetViewports(&iNumViewports, &Viewport);

    m_iViewWidth = Viewport.Width;
    m_iViewHeight = Viewport.Height;

    D3D11_TEXTURE2D_DESC		TextureDesc{};
    TextureDesc.Width = Viewport.Width;
    TextureDesc.Height = Viewport.Height;
    TextureDesc.MipLevels = 1;
    TextureDesc.ArraySize = 1;
    TextureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
    TextureDesc.SampleDesc.Quality = 0;
    TextureDesc.SampleDesc.Count = 1;
    TextureDesc.Usage = D3D11_USAGE_STAGING;
    TextureDesc.BindFlags = 0;
    TextureDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE;
    TextureDesc.MiscFlags = 0;

    if (FAILED(m_pDevice->CreateTexture2D(&TextureDesc, nullptr, &m_pTexture2D)))
        return E_FAIL;

    return S_OK;
}

void CPicking::Copy(const _wstring& strTargetTag)
{
    m_pGameInstance->Copy_RenderTarget(strTargetTag, m_pTexture2D);
}

_bool CPicking::Picking(_float3* pOut)
{
    _float4 vMousePos{};
    POINT ptMouse{};

    GetCursorPos(&ptMouse);
    ScreenToClient(m_hWnd, &ptMouse);
    D3D11_MAPPED_SUBRESOURCE        SubResource{};
    m_pContext->Map(m_pTexture2D, 0, D3D11_MAP_READ, 0, &SubResource);

    _uint iIndex = ptMouse.y * m_iViewWidth + ptMouse.x;

    _float fProjZ = { };

    if (0.f == static_cast<_float4*>(SubResource.pData)[iIndex].w)
    {
        m_pContext->Unmap(m_pTexture2D, 0);
        return false;
    }

    fProjZ = static_cast<_float4*>(SubResource.pData)[iIndex].x;

    m_pContext->Unmap(m_pTexture2D, 0);

    vMousePos.x = ptMouse.x / (m_iViewWidth * 0.5f) - 1.f;
    vMousePos.y = ptMouse.y / (m_iViewHeight * -0.5f) + 1.f;
    vMousePos.z = fProjZ;
    vMousePos.w = 1;

    XMStoreFloat4(&vMousePos, XMVector3TransformCoord(XMLoadFloat4(&vMousePos), m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_PROJ)));
    XMStoreFloat4(&vMousePos, XMVector3TransformCoord(XMLoadFloat4(&vMousePos), m_pGameInstance->Get_Transform_Inverse_Matrix(CPipeLine::D3DTS_VIEW)));

    memcpy(pOut, &vMousePos, sizeof(_float3));

    return true;
}

CPicking* CPicking::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, HWND hWnd)
{
    CPicking* pInstance = new CPicking(pDevice, pContext);

    if (FAILED(pInstance->Initialize(hWnd)))
    {
        MSG_BOX("Failed To Created : CPicking");
        Safe_Release(pInstance);
    }

    return pInstance;
}


void CPicking::Free()
{
    __super::Free();

    Safe_Release(m_pGameInstance);
    Safe_Release(m_pTexture2D);
    Safe_Release(m_pDevice);
    Safe_Release(m_pContext);
}
