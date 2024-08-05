#pragma once
#include "ConstantIndexOrVertexBuffer.h"

class ConstantVertexBuffer : public ConstantIndexOrVertexBuffer
{
private:
	D3D12_VERTEX_BUFFER_VIEW view;

public:
	ConstantVertexBuffer(ID3D12Device10* device, UINT size, UINT stride);
	void Copy(ID3D12Device10* device, ID3D12GraphicsCommandList7* list, BYTE* pData);
	D3D12_VERTEX_BUFFER_VIEW GetView() const;
};

