#include "pch.h"
#include "MainApp.h"

MainApp::MainApp()
{
}

HRESULT MainApp::Initialize()
{


    return S_OK;
}

void MainApp::Update()
{

}

HRESULT MainApp::Render()
{

    return S_OK;
}

MainApp* MainApp::Create()
{
    MainApp* pInstance = new MainApp;

    if (FAILED(pInstance->Initialize()))
    {
        delete pInstance;
        pInstance = nullptr;

        return nullptr;
    }

    return pInstance;
}

void MainApp::Free()
{
    __super::Free();
}
