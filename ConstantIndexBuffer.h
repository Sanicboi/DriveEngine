#pragma once
#include "ConstantIndexOrVertexBuffer.h"
class ConstantIndexBuffer : public ConstantIndexOrVertexBuffer
{
private:
	D3D12_INDEX_BUFFER_VIEW view;

public:
	using ConstantIndexOrVertexBuffer::ConstantIndexOrVertexBuffer;
	ConstantIndexBuffer(ID3D12Device10* device, uint64_t size);
	void Copy(ID3D12Device10* device, ID3D12GraphicsCommandList7* list, BYTE* pData);
	D3D12_INDEX_BUFFER_VIEW* GetView();
};

