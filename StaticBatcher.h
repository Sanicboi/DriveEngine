#pragma once
#include "StaticBatch.h"
class StaticBatcher : public Hresultable
{
private:
	std::vector<StaticBatch> batches;
	uint32_t m_numObjects;
	uint8_t	m_stride;
	uint32_t batchIdx = 0;
public:
	StaticBatcher(uint32_t numObjects = 0, uint8_t stride = 3 * sizeof(float));
	void Add(float* verts, uint64_t numVerts, uint32_t* indices, uint64_t numIndices);
	void Upload(ID3D12Device10* device, ID3D12GraphicsCommandList7* list);
	uint32_t GetNumBatches() const;
	void DrawCall(ID3D12GraphicsCommandList7* list);
	void Reset();
};

