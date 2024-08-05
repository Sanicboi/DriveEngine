#include "SwapChain.h"

SwapChain::SwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* queue, HWND hwnd)
{
	descriptor.BufferCount = 3;
	descriptor.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	descriptor.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	descriptor.SampleDesc.Count = 1;
	descriptor.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	hr = factory->CreateSwapChainForHwnd(queue, hwnd, &descriptor, NULL, NULL, &tmp);
	if (Ok()) {
		hr = tmp->QueryInterface<IDXGISwapChain4>(&ptr);
	}
}

