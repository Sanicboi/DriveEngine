#include "DepthStencilBuffer.h"

DepthStencilBuffer::DepthStencilBuffer(ID3D12Device10* device, uint16_t width, uint16_t height)
{
	heap = DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE_DSV, 1, false, device, true, false);
	hr = heap.GetHr();
	if (Ok()) {
		auto props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
		auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, width, height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
		D3D12_CLEAR_VALUE value = {};
		value.DepthStencil.Depth = 1.0f;
		value.DepthStencil.Stencil = 0.0f;
		value.Format = DXGI_FORMAT_D32_FLOAT;

		hr = device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &value, IID_PPV_ARGS(&buffer));

		D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
		viewDesc.Format = DXGI_FORMAT_D32_FLOAT;
		viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
		viewDesc.Flags = D3D12_DSV_FLAG_NONE;

		device->CreateDepthStencilView(buffer.Get(), &viewDesc, heap.cpuHandle);
	}
}

void DepthStencilBuffer::Resize(ID3D12Device10* device, uint16_t w, uint16_t h)
{
	buffer.Reset();
	auto props = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	auto desc = CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, w, h, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL);
	D3D12_CLEAR_VALUE value = {};
	value.DepthStencil.Depth = 1.0f;
	value.DepthStencil.Stencil = 0.0f;
	value.Format = DXGI_FORMAT_D32_FLOAT;

	hr = device->CreateCommittedResource(&props, D3D12_HEAP_FLAG_NONE, &desc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &value, IID_PPV_ARGS(&buffer));

	D3D12_DEPTH_STENCIL_VIEW_DESC viewDesc = {};
	viewDesc.Format = DXGI_FORMAT_D32_FLOAT;
	viewDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	viewDesc.Flags = D3D12_DSV_FLAG_NONE;

	device->CreateDepthStencilView(buffer.Get(), &viewDesc, heap.cpuHandle);
}
