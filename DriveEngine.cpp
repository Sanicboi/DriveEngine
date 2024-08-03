#include "Main.h"
#include "DXGIContext.h"
#include "Device.h"
#include "CommandQueue.h"
#include "Window.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"
#include "RenderTargetView.h"
#include "DoubleAllocatorList.h"
#include "VertexShader.h"
#include "PixelShader.h"


int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE prev, 
    _In_ PWSTR cmdLine,
    _In_ int nCmdShow
) 
{
    OutputDebugStringA("Hello world!\n");
    DXGIContext context;
    Device device(context.Adapter());
    CommandQueue queue(device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
    Window window(hInstance, L"Drive", 700, 600);
    window.Show(nCmdShow);
    SwapChain swapChain(context.Factory(), queue.Get(), window.GetHWND());
    DescriptorHeap rtvHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 3, false, device.Get(), true, false);
    ComPtr<ID3D12Resource2> renderTargets[3];
    std::vector<RenderTargetView> rtvs;
    for (int i = 0; i < 3; i++) {
        swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
        rtvs.push_back(
            RenderTargetView(
                device.Get(),
                renderTargets[i].Get(),
                rtvHeap.cpuHandle
            )
        );
        rtvHeap.cpuHandle.Offset(1, rtvHeap.size);
    }
    DoubleAllocatorList list(device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT, true);
    ShaderCompiler compiler;
    VertexShader vs(L"Vertex.hlsl", &compiler);
    PixelShader ps(L"Pixel.hlsl", &compiler);



    MSG msg = {};
    while (GetMessage(&msg, window.GetHWND(), 0, 0) > 0) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}