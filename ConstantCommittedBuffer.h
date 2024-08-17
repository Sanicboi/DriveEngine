#pragma once
#include "SinglePtr.h"
class ConstantCommittedBuffer : public SinglePtr<ID3D12Resource2>
{
public:
	using SinglePtr::SinglePtr;

	ConstantCommittedBuffer(ID3D12Device10* device, uint64_t size);
	void Map(void** ppV);
	void Unmap();
	UINT64 GetAddress();
};

