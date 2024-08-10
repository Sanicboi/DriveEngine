#pragma once
#include "DXGIContext.h"
#include "Device.h"
#include "CommandQueue.h"
#include "SwapChain.h"
#include "DescriptorHeap.h"
#include "DoubleAllocatorList.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "RootSignature.h"
#include "PSO.h"
#include "Fence.h"
#include "ConstantVertexBuffer.h"
#include "ConstantIndexBuffer.h"
#include "DepthStencilBuffer.h"
#include "Camera.h"
#include "ConstantCommittedBuffer.h"
#include "Timer.h"
#include "DirectXTK12/Inc/Mouse.h"
#include "Output.h"


struct Vertex {
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float a;
};


class Application : public Hresultable
{
private:
	DXGIContext context;
	Device device;
    Output output;
    CommandQueue queue;
    SwapChain swapChain;
    DescriptorHeap rtvHeap;
    ComPtr<ID3D12Resource2> renderTargets[3];
    DoubleAllocatorList list;
    ShaderCompiler compiler;
    RootSignature rs;
    PSO pso;
    Fence fence;
    D3D12_VIEWPORT viewport;
    D3D12_RECT scissorRect;
    HWND hwnd;

    UINT width, height;
    UINT frameIndex = 0;
    
    ConstantVertexBuffer vertexBuffer;
    ConstantIndexBuffer indexBuffer;
    DepthStencilBuffer dsBuffer;
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW indexBufferView;


    Camera camera;
    ConstantCommittedBuffer matrixBuffer;
    Timer timer;
    float deltaT;
    std::unique_ptr<Mouse> mouse = std::make_unique<Mouse>();
    

    bool ready = false;

public:
    bool running = true;
    Application(LPCWSTR name, HINSTANCE hInstance, int param);
    void Init(LPCWSTR name, HINSTANCE hInstance, int param);
    void OnRender();
    void Wait();
    void Signal();
    void OnResize(UINT w, UINT h);
    void OnKeyDown(UINT key);
    void OnMouseMove(LPARAM param);
    bool IsKeyPressed(UINT key);
    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
    
};

