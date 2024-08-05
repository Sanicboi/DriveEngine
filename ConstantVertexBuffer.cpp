#include "ConstantVertexBuffer.h"

ConstantVertexBuffer::ConstantVertexBuffer(ID3D12Device10* device, UINT size, UINT stride) : ConstantIndexOrVertexBuffer(device, size, stride)
{
	if (Ok()) {
		view.BufferLocation = ptr->GetGPUVirtualAddress();
		view.SizeInBytes = size;
		view.StrideInBytes = stride;
	}
}

void ConstantVertexBuffer::Copy(ID3D12Device10* device, ID3D12GraphicsCommandList7* list, BYTE* pData)
{
	ConstantIndexOrVertexBuffer::Copy(device, list, pData, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER);
}


D3D12_VERTEX_BUFFER_VIEW ConstantVertexBuffer::GetView() const
{
	return view;
}
