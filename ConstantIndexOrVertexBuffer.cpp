#include "ConstantIndexOrVertexBuffer.h"

ConstantIndexOrVertexBuffer::ConstantIndexOrVertexBuffer(ID3D12Device10* device, UINT size, UINT stride) : desc(CD3DX12_RESOURCE_DESC::Buffer(size))
{
	hr = device->CreateCommittedResource(&bufferHeapProperties, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&ptr));
}

void ConstantIndexOrVertexBuffer::Copy(ID3D12Device10* device, ID3D12GraphicsCommandList7* list, BYTE* pData, D3D12_RESOURCE_STATES state)
{
	auto heapProps = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);

	hr = device->CreateCommittedResource(&heapProps, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_COMMON, NULL, IID_PPV_ARGS(&tempBuffer));
	if (Ok()) {
		D3D12_SUBRESOURCE_DATA data = {};
		data.pData = pData;
		data.RowPitch = desc.Width;
		data.SlicePitch = desc.Width;

		D3D12_RESOURCE_BARRIER barriers[] = {
			CD3DX12_RESOURCE_BARRIER::Transition(ptr.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST),
			CD3DX12_RESOURCE_BARRIER::Transition(tempBuffer.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_SOURCE)
		};
		
		list->ResourceBarrier(2, barriers);


		UpdateSubresources(list, ptr.Get(), tempBuffer.Get(), 0, 0, 1, &data);

		auto barrier = CD3DX12_RESOURCE_BARRIER::Transition(ptr.Get(), D3D12_RESOURCE_STATE_COPY_DEST, state);

		list->ResourceBarrier(1, &barrier);
	}
}

void ConstantIndexOrVertexBuffer::ReleaseTemp()
{
	tempBuffer.Reset();
}
