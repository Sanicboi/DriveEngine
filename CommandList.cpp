#include "CommandList.h"

CommandList::CommandList(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* allocator, ID3D12PipelineState* pso, bool close)
{
	if (close) {
		hr = device->CreateCommandList1(0, type, D3D12_COMMAND_LIST_FLAG_NONE, IID_PPV_ARGS(&ptr));
	}
	else {
		hr = device->CreateCommandList(0, type, allocator, pso, IID_PPV_ARGS(&ptr));
	}
}
