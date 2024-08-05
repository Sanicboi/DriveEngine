#include "ConstantIndexBuffer.h"

ConstantIndexBuffer::ConstantIndexBuffer(ID3D12Device10* device, UINT size): ConstantIndexOrVertexBuffer(device, size, sizeof(UINT))
{
	if (Ok()) {
		view.BufferLocation = ptr->GetGPUVirtualAddress();
		view.Format = DXGI_FORMAT_R32_UINT;
		view.SizeInBytes = size;
	}
}

void ConstantIndexBuffer::Copy(ID3D12Device10* device, ID3D12GraphicsCommandList7* list, BYTE* pData)
{
	ConstantIndexOrVertexBuffer::Copy(device, list, pData, D3D12_RESOURCE_STATE_INDEX_BUFFER);
}

D3D12_INDEX_BUFFER_VIEW ConstantIndexBuffer::GetView() const
{
	return view;
}
