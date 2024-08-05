#include "Application.h"

Application::Application(LPCWSTR name, HINSTANCE hInstance, int param)
{
	Init(name, hInstance, param);
}

void Application::Init(LPCWSTR name, HINSTANCE hInstance, int param)
{
	context = DXGIContext();
	device = Device(context.Adapter());
	queue = CommandQueue(device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT);


	const LPCWSTR className = (std::wstring(name) + L" Window").c_str();
	WNDCLASS wc = {};
	wc.hInstance = hInstance;
	wc.lpfnWndProc = Application::WindowProc;
	wc.lpszClassName = className;
	RegisterClass(&wc);

	hwnd = CreateWindowExW(
		0,
		className,
		name,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		800, 600,
		NULL, NULL, hInstance, this
	);

	width = 800;
	height = 600;

	ShowWindow(hwnd, param);

	swapChain = SwapChain(context.Factory(), queue.Get(), hwnd);

	rtvHeap = DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_RTV, 3, false, device.Get(), true, false);

	for (int i = 0; i < 3; i++) {
		swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
		device->CreateRenderTargetView(renderTargets[i].Get(), NULL, rtvHeap.cpuHandle);
		rtvHeap.cpuHandle.Offset(1, rtvHeap.size);
	}

	list = DoubleAllocatorList(device.Get(), D3D12_COMMAND_LIST_TYPE_DIRECT, false);
	rs = RootSignature(device.Get(), 0, NULL, 0, NULL, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT, D3D_ROOT_SIGNATURE_VERSION_1);

	VertexShader vs(L"Vertex.hlsl", &compiler);
	PixelShader ps(L"Pixel.hlsl", &compiler);

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


	pso = PSO(&psoDesc, device.Get());

	Vertex verts[] = {
	{ 0.0f, 0.25f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f  },
	{ 0.25f, -0.25f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },
	{ -0.25f, -0.25f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f }
	};

	UINT indices[] = {
	0, 1, 2
	};

	fence = Fence(device.Get());
	vertexBuffer = ConstantVertexBuffer(device.Get(), sizeof(verts), sizeof(Vertex));
	indexBuffer = ConstantIndexBuffer(device.Get(), sizeof(indices));

	vertexBuffer.Copy(device.Get(), list.Get(), reinterpret_cast<BYTE*>(verts));
	indexBuffer.Copy(device.Get(), list.Get(), reinterpret_cast<BYTE*>(indices));

	list->Close();

	indexBufferView = indexBuffer.GetView();
	vertexBufferView = vertexBuffer.GetView();

	ID3D12CommandList* ppLists[] = { list.Get() };
	queue->ExecuteCommandLists(1, ppLists);

	Signal();
	Wait();

	list.ResetCurrent();
	list.ResetList(pso.Get());

	vertexBuffer.ReleaseTemp();
	indexBuffer.ReleaseTemp();

	viewport = D3D12_VIEWPORT(0.0f, 0.0f, 800.0f, 600.0f);
	scissorRect = D3D12_RECT(0, 0, 800, 600);
	
	ready = true;
}

void Application::OnRender()
{
	if (ready) {
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
		list->IASetVertexBuffers(0, 1, &vertexBufferView);
		list->IASetIndexBuffer(&indexBufferView);
		list->DrawIndexedInstanced(3, 1, 0, 0, 0);
		barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		list->ResourceBarrier(1, &barrier);
		list->Close();


		Wait();
		list.Reset();
		fence.value++;

		ID3D12CommandList* ppLists[] = { list.Get() };
		queue->ExecuteCommandLists(1, ppLists);
		swapChain->Present(1, 0);
		frameIndex = swapChain->GetCurrentBackBufferIndex();
		Signal();
		list.Switch(pso.Get());
	}
	
}

void Application::Wait()
{
	if (fence->GetCompletedValue() < fence.value) {
		fence.SetEvent();
		WaitForSingleObject(fence.event, INFINITE);
	}
}

void Application::Signal()
{
	queue->Signal(fence.Get(), fence.value);
}

void Application::OnResize(UINT w, UINT h)
{
	if (ready) {
		Wait();

		width = w;
		height = h;
		scissorRect = D3D12_RECT(0, 0, w, h);
		viewport = D3D12_VIEWPORT(0.0f, 0.0f, (float)w, (float)h);

		for (int i = 0; i < 3; i++) {
			renderTargets[i].Reset();
		}
		rtvHeap.ResetCpuHandle(0);
		swapChain->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);

		for (int i = 0; i < 3; i++) {
			swapChain->GetBuffer(i, IID_PPV_ARGS(&renderTargets[i]));
			device->CreateRenderTargetView(renderTargets[i].Get(), NULL, rtvHeap.cpuHandle);
			rtvHeap.cpuHandle.Offset(1, rtvHeap.size);
		}

		frameIndex = swapChain->GetCurrentBackBufferIndex();
	}
}

LRESULT Application::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	Application* app = reinterpret_cast<Application*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));

	switch (msg) {
		case WM_CREATE:
		{
			LPCREATESTRUCT pCreateStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
			SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pCreateStruct->lpCreateParams));
			return 0;
		}
		case WM_PAINT:
			if (app != NULL) {
				app->OnRender();
			}
			return 0;
		case WM_SIZE:
		{
			UINT width = LOWORD(lParam);
			UINT height = HIWORD(lParam);
			app->OnResize(width, height);
			return 0;
		}
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
	}


	
	return DefWindowProc(hwnd, msg, wParam, lParam);
}