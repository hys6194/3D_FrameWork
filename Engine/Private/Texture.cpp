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
}

HRESULT Texture::Initialize_Prototype()
{
    return E_NOTIMPL;
}

HRESULT Texture::Initialize(void* pArg)
{
    return E_NOTIMPL;
}

Texture* Texture::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

Component* Texture::Clone(void* pArg)
{
    return nullptr;
}

void Texture::Free()
{
}
