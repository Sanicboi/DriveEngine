#pragma once
#include "Described.h"
class SwapChain : public Described<IDXGISwapChain4, DXGI_SWAP_CHAIN_DESC1>
{
public:
	SwapChain(IDXGIFactory7* factory, ID3D12CommandQueue* queue, HWND hwnd, int flag);
};

