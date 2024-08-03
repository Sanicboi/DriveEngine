#include "DescriptorHeap.h"

DescriptorHeap::DescriptorHeap(D3D12_DESCRIPTOR_HEAP_TYPE type, UINT numDescriptors, bool shaderVisible, ID3D12Device10* device, bool createCpuHandle, bool createGpuHandle)
{
	descriptor.Type = type;
	descriptor.NumDescriptors = numDescriptors;
	descriptor.Flags = shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;;
	hr = device->CreateDescriptorHeap(&descriptor, IID_PPV_ARGS(&ptr));

	if (Ok()) {
		size = device->GetDescriptorHandleIncrementSize(type);
		hasCpu = createCpuHandle;
		hasGpu = createGpuHandle;
		if (createCpuHandle) {
			cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(ptr->GetCPUDescriptorHandleForHeapStart());
		}
		if (createGpuHandle) {
			gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(ptr->GetGPUDescriptorHandleForHeapStart());
		}
	}
	else {
		hasCpu = false;
		hasGpu = false;
		size = 0;
	}
}

bool DescriptorHeap::HasCpu() const
{
	return hasCpu;
}

bool DescriptorHeap::HasGpu() const
{
	return hasGpu;
}

void DescriptorHeap::ResetCpuHandle(UINT offset = 0)
{
	cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(ptr->GetCPUDescriptorHandleForHeapStart(), offset, size);
}
