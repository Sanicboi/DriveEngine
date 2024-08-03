#include "PSO.h"

PSO::PSO(D3D12_GRAPHICS_PIPELINE_STATE_DESC* desc, ID3D12Device10* device)
{
	hr = device->CreateGraphicsPipelineState(desc, IID_PPV_ARGS(&ptr));
	descriptor = *desc;
}
