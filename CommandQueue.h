#pragma once
#include "Described.h"
class CommandQueue : public Described<ID3D12CommandQueue, D3D12_COMMAND_QUEUE_DESC>
{
public:
	CommandQueue(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type);
};

