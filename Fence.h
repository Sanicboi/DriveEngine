#pragma once
#include "SinglePtr.h"

class Fence: public SinglePtr<ID3D12Fence1>
{
public:
	using SinglePtr::SinglePtr;
	Fence(ID3D12Device10* device, D3D12_FENCE_FLAGS flags = D3D12_FENCE_FLAG_NONE);
	UINT value = 0;
	HANDLE event;
	void SetEvent();
};

