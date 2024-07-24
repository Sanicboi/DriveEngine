#include "Device.h"

Device::Device(IDXGIAdapter4* adapter) {
	hr = D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_2, IID_PPV_ARGS(&ptr));
}