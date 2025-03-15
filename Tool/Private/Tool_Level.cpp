#include "Create_Level.h"
#include "Tool_Level.h"

Tool_Level::Tool_Level(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
}

HRESULT Tool_Level::Initialize()
{
    return E_NOTIMPL;
}

void Tool_Level::Update(_float fTimeDelta)
{
}

HRESULT Tool_Level::Render()
{
    return E_NOTIMPL;
}

HRESULT Tool_Level::Ready_Layer_BackGround(const _tchar* pLayerTag)
{
    return E_NOTIMPL;
}

HRESULT Tool_Level::Ready_Layer_Camera(const _tchar* pLayerTag)
{
    return E_NOTIMPL;
}

HRESULT Tool_Level::Ready_Layer_Monster(const _tchar* pLayerTag)
{
    return E_NOTIMPL;
}

HRESULT Tool_Level::Ready_Layer_Player(const _tchar* pLayerTag)
{
    return E_NOTIMPL;
}

HRESULT Tool_Level::Ready_Lights()
{
    return E_NOTIMPL;
}

void Tool_Level::Update()
{
}

Tool_Level* Tool_Level::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
    return nullptr;
}

void Tool_Level::Free()
{
}
