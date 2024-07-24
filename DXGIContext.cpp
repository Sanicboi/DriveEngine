#include "DXGIContext.h"

DXGIContext::DXGIContext() {
	hr = CreateDXGIFactory(IID_PPV_ARGS(&factory));
	if (Ok()) {
		hr = factory->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&adapter));
	}
}

IDXGIAdapter4* DXGIContext::Adapter() {
	return adapter.Get();
}