#pragma once
#include "CommandAllocator.h"
#include "CommandList.h"

class DoubleAllocatorList: public Hresultable
{
private:
	CommandAllocator allocators[2];
	CommandList list;
	bool current = 0;
public:
	DoubleAllocatorList(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type, bool close = false);
	ID3D12GraphicsCommandList7* operator->();
	void Switch(ID3D12PipelineState* pso = NULL); // Reset list, Switch allocator
	void Reset(); // Reset the opposite allocator, No switch
};

