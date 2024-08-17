#pragma once
#include "Described.h"
class DescriptorHeap : public Described<ID3D12DescriptorHeap, D3D12_DESCRIPTOR_HEAP_DESC>
{
private:
	bool hasGpu, hasCpu;
public:
	using Described::Described;
	DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, uint16_t numDescriptors, bool shaderVisible, ID3D12Device10* device, bool createCpuHandle, bool createGpuHandle);
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuHandle;
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuHandle;
	uint32_t size;
	bool HasCpu() const;
	bool HasGpu() const;
	void ResetCpuHandle(uint16_t offset);
};

