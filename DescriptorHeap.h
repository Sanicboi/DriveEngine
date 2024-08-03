#pragma once
#include "Described.h"
class DescriptorHeap : public Described<ID3D12DescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC>
{
private:
	bool hasGpu, hasCpu;
public:
	DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible, ID3D12Device10* device, bool createCpuHandle, bool createGpuHandle);
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	UINT size;
	bool HasCpu() const;
	bool HasGpu() const;
	void ResetCpuHandle(UINT offset);
};

