#pragma once
#include "Main.h"
#include "Hresultable.h"
class DXGIContext: public Hresultable
{
private:
	ComPtr<IDXGIFactory7> factory;
	ComPtr<IDXGIAdapter4> adapter;
public:
	DXGIContext();
	IDXGIAdapter4* Adapter();
};

