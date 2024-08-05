#pragma once
#include "SinglePtr.h"
class CommandList : public SinglePtr<ID3D12GraphicsCommandList7>
{
public:
	using SinglePtr::SinglePtr;
	CommandList(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type, ID3D12CommandAllocator* allocator, ID3D12PipelineState* pso, bool close = true);
};

