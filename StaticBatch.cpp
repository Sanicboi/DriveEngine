#include "StaticBatch.h"

StaticBatch::StaticBatch(uint8_t stride)
{
	m_stride = stride;
}

void StaticBatch::Add(float* verts, uint64_t numVerts, uint32_t* indices, uint64_t numIndices)
{
	numObjects++;

	// Copy the vertices
	uint64_t old = m_verts.size();
	m_verts.resize(old + numVerts * m_stride / sizeof(float));
	uint64_t size = numVerts * m_stride;
	memcpy(m_verts.data() + old, verts, size);

	// Process the indices
	uint64_t offset = m_indices.size();
	for (uint64_t i = 0; i < numIndices; i++) {
		m_indices.push_back(indices[i] + offset);
	}
}

void StaticBatch::Create(ID3D12Device10* device)
{
	if (!done) {
		vb = ConstantVertexBuffer(device, m_verts.size() * sizeof(float), m_stride);
		ib = ConstantIndexBuffer(device, m_indices.size() * sizeof(uint32_t));
		done = true;
	}
}

D3D12_VERTEX_BUFFER_VIEW* StaticBatch::GetVertexBufferView()
{
	return vb.GetView();
}

D3D12_INDEX_BUFFER_VIEW* StaticBatch::GetIndexBufferView()
{
	return ib.GetView();
}

void StaticBatch::Upload(ID3D12GraphicsCommandList7* list, ID3D12Device10* device)
{
	if (done) {
		vb.Copy(device, list, reinterpret_cast<BYTE*>(m_verts.data()));
		ib.Copy(device, list, reinterpret_cast<BYTE*>(m_indices.data()));
	}
}

void StaticBatch::Release()
{
	vb.ReleaseTemp();
	ib.ReleaseTemp();
}

uint64_t StaticBatch::GetNumIndices()
{
	return m_indices.size();
}

uint64_t StaticBatch::GetNumVertices()
{
	return m_verts.size() / m_stride;
}

uint32_t StaticBatch::GetNumObjects()
{
	return numObjects;
}
