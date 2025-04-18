#include "Main.h"
#include "Application.h"




int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE prev, 
    _In_ PWSTR cmdLine,
    _In_ int32_t nCmdShow
) 
{
    CoInitialize(NULL);


    #ifdef _DEBUG
    ComPtr<ID3D12Debug6> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        debugController->SetEnableGPUBasedValidation(TRUE);
        debugController->SetEnableSynchronizedCommandQueueValidation(TRUE);
    }

#endif // _DEBUG

    Application app(L"Drive", hInstance, nCmdShow);

    MSG msg = {};
    while (app.running) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            app.OnRender();
        }

    }
    app.Signal();
    app.Wait();
    CoUninitialize();
    return 0;
}