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
	DepthStencilBuffer(ID3D12Device10* device, UINT width, UINT height);
	void Resize(ID3D12Device10* device, UINT w, UINT h);
};

