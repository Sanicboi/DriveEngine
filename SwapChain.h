#pragma once
#include "Described.h"
class SwapChain : public Described<IDXGISwapChain4, DXGI_SWAP_CHAIN_DESC1>
{
private:
	ComPtr<IDXGISwapChain1> tmp;
public:
	SwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* queue, HWND hwnd);
	~SwapChain();
};

