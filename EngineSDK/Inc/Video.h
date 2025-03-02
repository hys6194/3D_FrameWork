#pragma once


#include "Component.h"

BEGIN(Engine)

class ENGINE_DLL Video final : public Component
{
private:
	Video(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
	Video(const Video& Prototype);
	virtual ~Video() = default;

public:
	HRESULT Initialize_Prototype(const _tchar* pVideoFilePath);
	HRESULT Initialize(void* pArg);
    void    Update(_float fTimeDelta);


private:
	IGraphBuilder* pGraph = NULL;
	IMediaControl* pControl = NULL;
	IMediaEvent* pEvent = NULL;

public:
	// video가 있는 파일의 위치를 받아옴
	static Video* Create(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const _tchar* pVideoFilePath);
	virtual Component* Clone(void* pArg) override;
	virtual void Free()override;

};

END


/*
#include <dshow.h>
#include <windows.h>

#pragma comment(lib, "strmiids.lib")  // DirectShow 라이브러리 링크

// 전역 변수
IGraphBuilder* pGraph = NULL;
IMediaControl* pControl = NULL;
IMediaEvent* pEvent = NULL;

// 미디어 재생 함수
void PlayMedia(HWND hwnd, const wchar_t* filename) {
    // COM 라이브러리 초기화
    CoInitialize(NULL);

    // 필터 그래프 생성
    CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IGraphBuilder, (void**)&pGraph);

    // 미디어 컨트롤 인터페이스 가져오기
    pGraph->QueryInterface(IID_IMediaControl, (void**)&pControl);
    pGraph->QueryInterface(IID_IMediaEvent, (void**)&pEvent);

    // 파일 로드
    pGraph->RenderFile(filename, NULL);

    // 비디오를 부모 윈도우에 출력
    IVideoWindow* pVidWin = NULL;
    pGraph->QueryInterface(IID_IVideoWindow, (void**)&pVidWin);
    pVidWin->put_Owner((OAHWND)hwnd);
    pVidWin->put_WindowStyle(WS_CHILD | WS_CLIPSIBLINGS);
    pVidWin->SetWindowPosition(0, 0, 800, 600);
    pVidWin->Release();

    // 재생 시작
    pControl->Run();
}

// 윈도우 프로시저
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_CLOSE:
        if (pControl) pControl->Stop();
        if (pEvent) pEvent->Release();
        if (pControl) pControl->Release();
        if (pGraph) pGraph->Release();
        CoUninitialize();
        DestroyWindow(hwnd);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

// 윈도우 생성 및 실행
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = L"DirectShowPlayer";
    RegisterClass(&wc);

    HWND hwnd = CreateWindow(L"DirectShowPlayer", L"DirectShow Video Player",
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hwnd, nCmdShow);

    // 미디어 재생
    PlayMedia(hwnd, L"C:\\example.mp4");

    // 메시지 루프
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return msg.wParam;
}*/
