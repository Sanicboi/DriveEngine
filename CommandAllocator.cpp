#include "CommandAllocator.h"

CommandAllocator::CommandAllocator(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type)
{
	hr = device->CreateCommandAllocator(type, IID_PPV_ARGS(&ptr));
}
