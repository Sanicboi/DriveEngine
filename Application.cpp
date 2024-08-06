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
	D3D12_ROOT_PARAMETER params[1] = {};
	params[0].Descriptor.RegisterSpace = 0;
	params[0].Descriptor.ShaderRegister = 0;
	params[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	params[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rs = RootSignature(device.Get(), 1, params, 0, NULL, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT | D3D12_ROOT_SIGNATURE_FLAG_CBV_SRV_UAV_HEAP_DIRECTLY_INDEXED, D3D_ROOT_SIGNATURE_VERSION_1);

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
	psoDesc.DepthStencilState.DepthEnable = TRUE;
	psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	psoDesc.DepthStencilState.StencilEnable = FALSE;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.SampleDesc.Count = 1;


	pso = PSO(&psoDesc, device.Get());

	Vertex verts[] = {
		// front face
		{ -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },

		// right side face
		{  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },

		// left side face
		{ -0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f },

		// back face
		{  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f },

		// top face
		{ -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ 0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{ 0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f },

		// bottom face
		{  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f },
		{ -0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f },
		{  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f },
		{ -0.5f, -0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 1.0f },
	};

	UINT indices[] = {
			0, 1, 2, // first triangle
	0, 3, 1, // second triangle

	// left face
	4, 5, 6, // first triangle
	4, 7, 5, // second triangle

	// right face
	8, 9, 10, // first triangle
	8, 11, 9, // second triangle

	// back face
	12, 13, 14, // first triangle
	12, 15, 13, // second triangle

	// top face
	16, 17, 18, // first triangle
	16, 19, 17, // second triangle

	// bottom face
	20, 21, 22, // first triangle
	20, 23, 21, // second triangle
	};

	fence = Fence(device.Get());
	dsBuffer = DepthStencilBuffer(device.Get(), width, height);
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
	
	camera = Camera(0.0f, 0.0f, -10.0f, width, height, 60.0f);
	matrixBuffer = ConstantCommittedBuffer(device.Get(), sizeof(XMFLOAT4X4) * 4);
	XMFLOAT4X4 matrices[3] = {
		//camera.GetViewMatrix(),
		//camera.GetProjectionMatrix()
	};
	float* pData;
	matrixBuffer.Map(reinterpret_cast<void**>(&pData));
	memcpy(pData, matrices, sizeof(matrices));
	matrixBuffer.Unmap();
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
		list->OMSetRenderTargets(1, &rtvHeap.cpuHandle, FALSE, &dsBuffer.heap.cpuHandle);
		list->ClearDepthStencilView(dsBuffer.heap.cpuHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, NULL);
		const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
		list->ClearRenderTargetView(rtvHeap.cpuHandle, clearColor, 0, NULL);
		list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		list->IASetVertexBuffers(0, 1, &vertexBufferView);
		list->IASetIndexBuffer(&indexBufferView);
		list->SetGraphicsRootConstantBufferView(0, matrixBuffer.GetAddress());
		list->DrawIndexedInstanced(36, 1, 0, 0, 0);
		barrier = CD3DX12_RESOURCE_BARRIER::Transition(renderTargets[frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
		list->ResourceBarrier(1, &barrier);
		list->Close();


		Wait();
		camera.UpdateMatrices();
		XMFLOAT4X4 view = camera.GetViewMatrix();
		XMFLOAT4X4 proj = camera.GetProjectionMatrix();
		XMFLOAT4X4 matrices[3] = {
		};


		XMStoreFloat4x4(&matrices[0], XMMatrixTranspose(XMMatrixIdentity()));
		XMStoreFloat4x4(&matrices[1], 
			XMLoadFloat4x4(&view)
		);
		XMStoreFloat4x4(&matrices[2], 
			XMLoadFloat4x4(&proj)
		);
		//XMStoreFloat4x4(&matrices[0], XMMatrixIdentity());
		//XMStoreFloat4x4(&matrices[1], XMMatrixIdentity());
		//XMStoreFloat4x4(&matrices[2], XMMatrixIdentity());
		float* pData;
		matrixBuffer.Map(reinterpret_cast<void**>(&pData));
		memcpy(pData, matrices, sizeof(matrices));
		matrixBuffer.Unmap();

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
		dsBuffer.Resize(device.Get(), w, h);
		rtvHeap.ResetCpuHandle(0);
		swapChain->ResizeBuffers(0, w, h, DXGI_FORMAT_UNKNOWN, 0);
		camera.Resize(w, h);
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
