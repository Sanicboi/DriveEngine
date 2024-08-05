#pragma once
#include "SinglePtr.h"
class ConstantIndexOrVertexBuffer : public SinglePtr<ID3D12Resource2>
{
protected:
	CD3DX12_HEAP_PROPERTIES bufferHeapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);
	CD3DX12_RESOURCE_DESC desc;
	ComPtr<ID3D12Resource2> tempBuffer;
public:
	ConstantIndexOrVertexBuffer(ID3D12Device10* device, UINT size, UINT vertexSize);
	void Copy(ID3D12Device10* device, ID3D12GraphicsCommandList7* list, BYTE* pData, D3D12_RESOURCE_STATES state);
	void ReleaseTemp();
};

