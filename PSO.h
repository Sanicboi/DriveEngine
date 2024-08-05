#pragma once
#include "Described.h"
class PSO : public Described<ID3D12PipelineState, D3D12_GRAPHICS_PIPELINE_STATE_DESC>
{
public:
	using Described::Described;
	PSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc, ID3D12Device10* device);
};

