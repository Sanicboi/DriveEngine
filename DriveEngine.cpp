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
#include "RootSignature.h"
#include "PSO.h"
#include "Fence.h"
#include "ConstantVertexBuffer.h"


struct Vertex {
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float a;
};

int WINAPI wWinMain(
    _In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE prev, 
    _In_ PWSTR cmdLine,
    _In_ int nCmdShow
) 
{
    CoInitialize(NULL);





    DXGIContext context;

    #ifdef _DEBUG
    ComPtr<ID3D12Debug6> debugController;
    if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
    {
        debugController->EnableDebugLayer();
        debugController->SetEnableGPUBasedValidation(TRUE);
        //debugController->SetEnableSynchronizedCommandQueueValidation(TRUE);
    }

#endif // _DEBUG

    Device device(context.Adapter());



    CommandQueue queue(device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);
    Window window(hInstance, L"Drive", 800, 600);
    window.Show(nCmdShow);
    SwapChain swapChain(context.Factory(), queue.Get(), window.GetHWND());

    DescriptorHeap rtvHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 3, false, device.Get(), true, false);
    ComPtr<ID3D12Resource2> renderTargets[3];
    std::vector<RenderTargetView> rtvs;
    for (int i = 0; i < 3; i++) {
        swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
        renderTargets[i]->SetName(L"Render target");
        rtvs.push_back(
            RenderTargetView(
                device.Get(),
                renderTargets[i].Get(),
                rtvHeap.cpuHandle
            )
        );
        rtvHeap.cpuHandle.Offset(1, rtvHeap.size);
    }
    DoubleAllocatorList list(device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT, false);
    ShaderCompiler compiler;
    VertexShader vs(L"Vertex.hlsl", &compiler);
    PixelShader ps(L"Pixel.hlsl", &compiler);
    RootSignature rs(device.Get(), 0, NULL, 0, NULL, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1);

    
    D3D12_INPUT_ELEMENT_DESC inputs[] = {
            { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
            { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
    };

    D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
    psoDesc.InputLayout = { inputs, _countof(inputs) };
    psoDesc.pRootSignature = rs.Get();
    psoDesc.VS = { reinterpret_cast<UINT8*>(vs->GetBufferPointer()), vs->GetBufferSize() };
    psoDesc.PS = { reinterpret_cast<UINT8*>(ps->GetBufferPointer()), ps->GetBufferSize() };
    psoDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
    psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
    psoDesc.DepthStencilState.DepthEnable = FALSE;
    psoDesc.DepthStencilState.StencilEnable = FALSE;
    psoDesc.SampleMask = UINT_MAX;
    psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
    psoDesc.NumRenderTargets = 1;
    psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
    psoDesc.SampleDesc.Count = 1;
    PSO pso(&psoDesc, device.Get());


    // TEMP BEGIN
    Vertex verts[] = {
        { 0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  },
        { 0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
        { -0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }
    };

    Fence fence(device.Get());
    fence.value++;

    ConstantVertexBuffer vBuf(device.Get(), sizeof(verts), sizeof(Vertex));
    vBuf.Copy(device.Get(), list.Get(), reinterpret_cast<BYTE*>(verts));
    list->Close();

    D3D12_VERTEX_BUFFER_VIEW view = vBuf.GetView();
    
    ID3D12CommandList* ppLists[] = { list.Get() };
    queue->ExecuteCommandLists(1, ppLists);

    HRESULT hr = queue->Signal(fence.Get(), fence.value);

    if (fence->GetCompletedValue() < fence.value) {
        fence.SetEvent();
        WaitForSingleObject(fence.event, INFINITE);
    }
    list.ResetCurrent();
    list.ResetList(pso.Get());

    vBuf.ReleaseTemp();
    

    UINT frameIndex = 0;
    

    D3D12_VIEWPORT viewport(0.0f, 0.0f, 800.0f, 600.0f);
    D3D12_RECT scissorRect(0, 0, 800, 600);

    MSG msg = {};
    while (GetMessage(&msg, 0/*window.GetHWND()*/, 0, 0) > 0) {


            list->SetGraphicsRootSignature(rs.Get());
            list->RSSetViewports(1, &viewport);
            list->RSSetScissorRects(1, &scissorRect);
            auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
            list->ResourceBarrier(1, &barrier);
            rtvHeap.ResetCpuHandle(frameIndex);
            list->OMSetRenderTargets(1, &rtvHeap.cpuHandle, FALSE, NULL);
            const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
            list->ClearRenderTargetView(rtvHeap.cpuHandle, clearColor, 0, NULL);
            list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
            list->IASetVertexBuffers(0, 1, &view);
            list->DrawInstanced(3, 1, 0, 0);
            barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
            list->ResourceBarrier(1, &barrier);
            list->Close();

            

            if (fence->GetCompletedValue() < fence.value) {
                fence.SetEvent();
                WaitForSingleObject(fence.event, INFINITE);
            }
            list.Reset();
            fence.value++;

            ID3D12CommandList* ppLists[] = { list.Get() };
            queue->ExecuteCommandLists(1, ppLists);
            swapChain->Present(1, 0);
            frameIndex = swapChain->GetCurrentBackBufferIndex();
            queue->Signal(fence.Get(), fence.value);
            list.Switch(pso.Get());

            TranslateMessage(&msg);
            DispatchMessage(&msg);
    }
    if (fence->GetCompletedValue() < fence.value) {
        fence.SetEvent();
        WaitForSingleObject(fence.event, INFINITE);
    }
    CoUninitialize();
    return 0;
}