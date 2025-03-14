#include "MyFont.h"

CMyFont::CMyFont(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : m_pDevice {pDevice}
    , m_pContext {pContext}
{
    Safe_AddRef(m_pDevice);
    Safe_AddRef(m_pContext);
}

HRESULT CMyFont::Initialize(const _tchar* pFontFilePath)
{
    m_pFont = new SpriteFont(m_pDevice, pFontFilePath);

    m_pBatch = new SpriteBatch(m_pContext);

    return S_OK;
}

HRESULT CMyFont::Render(const _wstring& strText, const _float2& vPosition, _fvector vColor, _float fRadian, const _float2& vOrigin, _float fScale)
{
    m_pBatch->Begin();

    m_pFont->DrawString(m_pBatch, strText.c_str(), vPosition, vColor, fRadian, vOrigin, fScale);

    m_pBatch->End();

    return S_OK;
}

CMyFont* CMyFont::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pFontFilePath)
{
    CMyFont* pInstance = new CMyFont(pDevice, pContext);

    if (FAILED(pInstance->Initialize(pFontFilePath)))
    {
        MSG_BOX("Failed To Created : MyFont");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CMyFont::Free()
{
    __super::Free();

    Safe_Delete(m_pBatch);
    Safe_Delete(m_pFont);

    Safe_Release(m_pContext);
    Safe_Release(m_pDevice);
}