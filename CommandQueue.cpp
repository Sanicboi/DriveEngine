#include "CommandQueue.h"

CommandQueue::CommandQueue(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type)
{
	descriptor.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	descriptor.NodeMask = 0;
	descriptor.Priority = D3D12_COMMAND_QUEUE_PRIORITY_HIGH;
	descriptor.Type = type;
	hr = device->CreateCommandQueue(&descriptor, IID_PPV_ARGS(&ptr));
}
