#include "Texture.h"
#include "Shader.h"

Texture::Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component { pDevice , pContext }
{
}

Texture::Texture(const Texture& Prototype)
    : Component{ Prototype }
    , m_iNumTextures{ Prototype.m_iNumTextures }
    , m_vecSRV{ Prototype.m_vecSRV }
{
    for (auto& pSRV : m_vecSRV)
        Safe_AddRef(pSRV);
}

HRESULT Texture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
    m_iNumTextures = iNumTextures;

    m_vecSRV.reserve(m_iNumTextures);

    // 경로를 받아올 tchar 자료형
    _tchar		szEXT[MAX_PATH] = {};

    _wsplitpath_s(pTextureFilePath, nullptr, 0, nullptr, 0, nullptr, 0, szEXT, MAX_PATH);

    for (size_t i = 0; i < iNumTextures; ++ i)
    {
        // 경로에서 서식문자를 답고있는것을 치환
        _tchar		szFullPath[MAX_PATH] = {};

        wsprintf(szFullPath, pTextureFilePath, i);

        ID3D11ShaderResourceView* pSRV = { nullptr };

        // dds 파일 읽기
        if (false == lstrcmp(szEXT, TEXT(".dds")))
        {
            if (FAILED(CreateDDSTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV)))
                return E_FAIL;
        }
        else if (false == lstrcmp(szEXT, TEXT(".tga")))
        {
            return E_FAIL;
        }

        // png 파일 읽기
        else
        {
            if (FAILED(CreateWICTextureFromFile(m_pDevice, szFullPath, nullptr, &pSRV)))
                return E_FAIL;
        }

        m_vecSRV.push_back(pSRV);
    }

    return S_OK;
}

HRESULT Texture::Initialize(void* pArg)
{
    return S_OK;
}

HRESULT Texture::Bind_SR(const _char* pConstantName, class Shader* pShader, _uint iTextureIndex) const
{
    if (iTextureIndex >= m_iNumTextures)
        return E_FAIL;


    return pShader->Bind_SRV(pConstantName, m_vecSRV[iTextureIndex]);
}

Texture* Texture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pTextureFilePath, _uint iNumTextures)
{
    Texture* pInstance = new Texture(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pTextureFilePath, iNumTextures)))
    {
        MSG_BOX("Failed To Created : Texture");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Component* Texture::Clone(void* pArg)
{
    Component* pInstance = new Texture(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Texture");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Texture::Free()
{
    __super::Free();

    for (auto& pSRV : m_vecSRV)
        Safe_Release(pSRV);

    m_vecSRV.clear();
}
