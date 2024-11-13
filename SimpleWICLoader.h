#pragma once
#include "Hresultable.h"

class SimpleWICLoader : public Hresultable
{
private:
	static DXGI_FORMAT GetDXGIFormatFromWicFormat(WICPixelFormatGUID& wicFormatGUID);
	static WICPixelFormatGUID GetConvertToWICFormat(WICPixelFormatGUID& wicFormatGUID);
	static int GetDXGIFormatBitsPerPixel(DXGI_FORMAT& dxgiFormat);
	ComPtr<IWICImagingFactory> factory;
public:
	SimpleWICLoader();
	uint32_t LoadImageDataFromFile(BYTE** data, D3D12_RESOURCE_DESC& desc, LPCWSTR file, uint32_t& bytesPerRow);
};

