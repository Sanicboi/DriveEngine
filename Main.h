#pragma once
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include "d3dx12.h"
#include <wrl.h>
#include <vector>
#include <dxcapi.h>
#include <d3dcompiler.h>
#include <string>

#ifdef _DEBUG 
#include <d3d12sdklayers.h>
#include <dxgidebug.h>
#endif

#define EXITNOTOK if (!Ok()) return
#define SETHR(o) hr = o.GetHr(); EXITNOTOK


using namespace Microsoft::WRL;
