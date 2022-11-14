#pragma once
#include <d3d12.h>
#include "d3dx12.h"
#include <cassert>
#include <wrl.h>

// ���_�o�b�t�@
template<class VERTEX_TYPE> class VertexBuffer 
{
	
private:
	UINT m_vertexCount = 0;
	UINT m_verticesByteSize = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer;
	VERTEX_TYPE* m_map = nullptr;
	D3D12_VERTEX_BUFFER_VIEW m_bufferView = {};

public:
	VertexBuffer() {}
	~VertexBuffer() {}

	void Create(ID3D12Device* device, UINT vertexCount)
	{
		m_vertexCount = vertexCount;
		m_verticesByteSize = static_cast<UINT>(sizeof(VERTEX_TYPE) * m_vertexCount);
		CreateBuffer(device);
		CreateView();
	}

	void Map()
	{
		HRESULT result = S_FALSE;
		result = m_buffer->Map(0, nullptr, (void**)&m_map); // �}�b�s���O
		assert(SUCCEEDED(result));
	}

	void Unmap() 
	{
		m_buffer->Unmap(0, nullptr);
	}

	VERTEX_TYPE* GetMapPtr() { return m_map; }
	UINT GetVertexCount() const { return m_vertexCount; }

	void IASet(ID3D12GraphicsCommandList* cmdList) 
	{
		cmdList->IASetVertexBuffers(0, 1, &m_bufferView);
	}

private:

	void CreateBuffer(ID3D12Device* device) 
	{
		HRESULT result = S_FALSE;
		
		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(m_verticesByteSize);
		// �o�b�t�@�̐���
		result = device->CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE,
			&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&m_buffer));
		assert(SUCCEEDED(result));
	}

	void CreateView() 
	{
		// GPU���z�A�h���X
		m_bufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
		m_bufferView.SizeInBytes = m_verticesByteSize;
		m_bufferView.StrideInBytes = sizeof(VERTEX_TYPE);
	}

};