#include "Texture.h"

Texture::Texture(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component { pDevice , pContext }
{
}

Texture::Texture(const Texture& Prototype)
    : Component{ Prototype }
    , m_iNumTextures{ Prototype.m_iNumTextures }
    , m_vecSRV{ Prototype.m_vecSRV }
{
    for (auto& iter : m_vecSRV)
    {
        Safe_AddRef(iter);
    }
}

HRESULT Texture::Initialize_Prototype(const _tchar* pTextureFilePath, _uint iNumTextures)
{
    m_iNumTextures = iNumTextures;

    m_vecSRV.reserve(m_iNumTextures);

    // 경로를 받아올 tchar 자료형
    _tchar		szEXT[MAX_PATH] = {};

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
            if (FAILED(CreateWICTextureFromFileEx(m_pDevice,
                szFullPath,
                MAX_PATH,
                D3D11_USAGE_DEFAULT,                           //Usage
                D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET,   //BindFlags : 리소스와 타깃 모두 사용가능
                0,                                          //CPUAccessFlags : Cpu Access 하지 않음
                D3D11_RESOURCE_MISC_GENERATE_MIPS,                  //MiscFlags
                WIC_LOADER_DEFAULT,                              //이부분은 수정이 필요할 수도 있음 -> WIC_LOADER_FORCE_SRGB 같은
                nullptr,
                &pSRV)))
                return E_FAIL;

            // 해상도를 낮춰가며 밉맵 생성
            m_pContext->GenerateMips(pSRV);
        }

        m_vecSRV.push_back(pSRV);
    }

    return S_OK;
}

HRESULT Texture::Initialize(void* pArg)
{
    return S_OK;
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

    for (auto& iter : m_vecSRV)
    {
        Safe_Release(iter);
    }

    m_vecSRV.clear();
}
