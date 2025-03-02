#include "Video.h"


Video::Video(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
    : Component { pDevice , pContext }
{
}

Video::Video(const Video& Prototype)
    : Component{ Prototype }
{

}

HRESULT Video::Initialize_Prototype(const _tchar* pVideoFilePath)
{
    CoInitialize(NULL);

    CoCreateInstance(CLSID_FilterGraph,
                     NULL, 
                     CLSCTX_INPROC_SERVER, 
                     IID_IGraphBuilder, 
                     (void**)&pGraph);

    pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);

    pGraph->RenderFile(pVideoFilePath, NULL);


    return S_OK;
}

HRESULT Video::Initialize(void* pArg)
{
    return S_OK;
}

void Video::Update(_float fTimeDelta)
{
}


Video* Video::Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pVideoFilePath)
{
    Video* pInstance = new Video(pDevice, pContext);

    if (FAILED(pInstance->Initialize_Prototype(pVideoFilePath)))
    {
        MSG_BOX("Failed To Created : Video");
        Safe_Release(pInstance);
    }

    return pInstance;
}

Component* Video::Clone(void* pArg)
{
    Component* pInstance = new Video(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX("Failed To Cloned : Video");
        Safe_Release(pInstance);
    }

    return pInstance;
}

void Video::Free()
{
    __super::Free();

}
