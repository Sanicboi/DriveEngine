#include "Fence.h"

Fence::Fence(ID3D12Device10* device, D3D12_FENCE_FLAGS flags)
{
	hr = device->CreateFence(0, flags, IID_PPV_ARGS(&ptr));
	if (Ok()) {
		event = CreateEvent(NULL, FALSE, FALSE, NULL);
		if (event == NULL) {
			hr = E_ABORT;
		}
	}
}

void Fence::SetEvent()
{
	hr = ptr->SetEventOnCompletion(value, event);
}
