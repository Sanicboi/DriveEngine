#pragma once
#include "Hresultable.h"
#include "DescriptorHeap.h"

class DepthStencilBuffer : public Hresultable
{
private:
	ComPtr<ID3D12Resource2> buffer;
public:
	DescriptorHeap heap;
	using Hresultable::Hresultable;
	DepthStencilBuffer(ID3D12Device10* device, uint16_t width, uint16_t height);
	void Resize(ID3D12Device10* device, uint16_t w, uint16_t h);
};

