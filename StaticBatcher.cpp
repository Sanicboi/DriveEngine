#include "StaticBatcher.h"

StaticBatcher::StaticBatcher(uint32_t numObjects, uint8_t stride): m_numObjects(numObjects), m_stride(stride)
{
}

void StaticBatcher::Add(float* verts, uint64_t numVerts, uint32_t* indices, uint64_t numIndices)
{
	if (batches.size() == 0) {
		batches.push_back(StaticBatch(m_stride));
		batches[0].Add(verts, numVerts, indices, numIndices);
	}
	else if (batches[batches.size() - 1].GetNumObjects() < m_numObjects) {
		batches[batches.size() - 1].Add(verts, numVerts, indices, numIndices);
	}
	else {
		batches.push_back(StaticBatch(m_stride));
		batches[batches.size() - 1].Add(verts, numVerts, indices, numIndices);
	}
	m_numObjects++;
}

void StaticBatcher::Upload(ID3D12Device10* device, ID3D12GraphicsCommandList7* list)
{
	for (int i = 0; i < batches.size(); i++) {
		batches[i].Create(device);
		batches[i].Upload(list, device);
	}
}

UINT StaticBatcher::GetNumBatches() const
{
	return batches.size();
}

void StaticBatcher::DrawCall(ID3D12GraphicsCommandList7* list)
{
	list->IASetVertexBuffers(0, 1, batches[batchIdx].GetVertexBufferView());
	list->IASetIndexBuffer(batches[batchIdx].GetIndexBufferView());
	list->DrawIndexedInstanced(batches[batchIdx].GetNumIndices(), 1, 0, 0, 0);
}

void StaticBatcher::Reset()
{
	batchIdx = 0;
}
