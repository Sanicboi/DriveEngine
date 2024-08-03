#include "RenderTargetView.h"

RenderTargetView::RenderTargetView(ID3D12Device10* device, ID3D12Resource2* buffer, CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle, D3D12_RENDER_TARGET_VIEW_DESC* desc)
{
	device->CreateRenderTargetView(buffer, desc, rtvHandle);
}
