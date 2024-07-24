#pragma once
#include "SinglePtr.h"
class Device: public SinglePtr<ID3D12Device10>
{
public:
	Device(IDXGIAdapter4* adapter);
};

