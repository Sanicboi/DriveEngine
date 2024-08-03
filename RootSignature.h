#pragma once
#include "Described.h"
class RootSignature : public Described<ID3D12RootSignature, CD3DX12_ROOT_SIGNATURE_DESC>
{
public:
	RootSignature(ID3D12Device10* device, UINT numParameters, const D3D12_ROOT_PARAMETER* pParams, UINT numStaticSamplers, const D3D12_STATIC_SAMPLER_DESC*, D3D12_ROOT_SIGNATURE_FLAGS flags, D3D_ROOT_SIGNATURE_VERSION version);
};

