#pragma once
#include "SinglePtr.h"
class CommandAllocator : public SinglePtr<ID3D12CommandAllocator>
{
public: 
	using SinglePtr::SinglePtr;
	CommandAllocator(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type);
};

