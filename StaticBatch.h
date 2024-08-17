#pragma once
#include "ConstantVertexBuffer.h"
#include "ConstantIndexBuffer.h"
class StaticBatch : public Hresultable
{
private:
	ConstantVertexBuffer vb;
	ConstantIndexBuffer ib;
	std::vector<float> m_verts;
	std::vector<uint32_t> m_indices;
	uint8_t m_stride = 0;
	bool done = false;
	uint32_t numObjects = 0;
public:
	StaticBatch(uint8_t stride = 3 * sizeof(float));
	void Add(float* verts, uint64_t numVerts, uint32_t* indices, uint64_t numIndices);
	void Create(ID3D12Device10* device);
	D3D12_VERTEX_BUFFER_VIEW* GetVertexBufferView();
	D3D12_INDEX_BUFFER_VIEW* GetIndexBufferView();
	void Upload(ID3D12GraphicsCommandList7* list, ID3D12Device10* device);
	void Release();
	uint64_t GetNumIndices();
	uint64_t GetNumVertices();
	uint32_t GetNumObjects();
};

