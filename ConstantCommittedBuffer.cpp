#include "ConstantCommittedBuffer.h"

ConstantCommittedBuffer::ConstantCommittedBuffer(ID3D12Device10* device, UINT size)
{
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	auto desc = CD3DX12_RESOURCE_DESC::Buffer(size);
	hr = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER, NULL, IID_PPV_ARGS(&ptr));
}

void ConstantCommittedBuffer::Map(void** ppV)
{
	D3D12_RANGE read(0, 0);
	ptr->Map(0, &read, ppV);
}

void ConstantCommittedBuffer::Unmap()
{
	ptr->Unmap(0, NULL);
}

UINT64 ConstantCommittedBuffer::GetAddress()
{
	return ptr->GetGPUVirtualAddress();
}
