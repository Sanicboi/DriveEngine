#include "RootSignature.h"

RootSignature::RootSignature(ID3D12Device10* device, UINT numParameters, const D3D12_ROOT_PARAMETER* pParams, UINT numStaticSamplers, const D3D12_STATIC_SAMPLER_DESC* pSamplers, D3D12_ROOT_SIGNATURE_FLAGS flags, D3D_ROOT_SIGNATURE_VERSION version)
{
	descriptor.Init(numParameters, pParams, numStaticSamplers, pSamplers, flags);
	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	hr = D3D12SerializeRootSignature(&descriptor, version, &signature, &error);
	if (Ok()) {
		hr = device->CreateRootSignature(0, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&ptr));
	}
}
