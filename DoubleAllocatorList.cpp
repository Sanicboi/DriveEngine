#include "DoubleAllocatorList.h"

DoubleAllocatorList::DoubleAllocatorList(ID3D12Device10* device, D3D12_COMMAND_LIST_TYPE type, bool close)
	: allocators{ CommandAllocator(device,type), CommandAllocator(device, type) }, list(device, type, allocators[current].Get(), NULL, close)
{
	allocators[0] = CommandAllocator(device, type);
	allocators[1] = CommandAllocator(device, type);
	if (allocators[0].Ok() && allocators[1].Ok()) hr = S_OK;
	else if (!allocators[0].Ok()) hr = allocators[0].GetHr();
	else hr = allocators[1].GetHr();
	if (Ok()) {
		if (close) {
			list = CommandList(device, type, NULL, NULL, close);
		}
		else {
			list = CommandList(device, type, allocators[current].Get(), NULL, false);
		}

		hr = list.GetHr();
	}
}

ID3D12GraphicsCommandList7* DoubleAllocatorList::operator->()
{
	return list.Get();
}


ID3D12GraphicsCommandList7* DoubleAllocatorList::Get()
{
	return list.Get();
}

void DoubleAllocatorList::Switch(ID3D12PipelineState* pso)
{
	current = !current;
	hr = list->Reset(allocators[current].Get(), pso);
}

void DoubleAllocatorList::Reset()
{
	hr = allocators[!current]->Reset();
}
