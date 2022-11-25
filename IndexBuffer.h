#pragma once
#include <d3d12.h>
#include "d3dx12.h"
#include <cassert>
#include <wrl.h>

// 頂点バッファ
class IndexBuffer
{

private:
	UINT m_indexCount;
	Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer;
	uint16_t* m_map = nullptr;
	D3D12_INDEX_BUFFER_VIEW m_bufferView = {};

public:
	IndexBuffer() {}
	~IndexBuffer() {}

	void Create(ID3D12Device* device, UINT indexCount)
	{
		m_indexCount = indexCount;
		UINT byteSize = static_cast<UINT>(sizeof(uint16_t) * m_indexCount);
		CreateBuffer(device, byteSize);
		CreateView(byteSize);
	}

	void Map()
	{
		HRESULT result = S_FALSE;
		result = m_buffer->Map(0, nullptr, (void**)&m_map); // マッピング
		assert(SUCCEEDED(result));
	}

	void Unmap()
	{
		m_buffer->Unmap(0, nullptr);
	}

	uint16_t* GetMapPtr() { return m_map; }
	UINT GetIndexCount() const { return m_indexCount; }

	void IASet(ID3D12GraphicsCommandList* cmdList)
	{
		cmdList->IASetIndexBuffer(&m_bufferView);
	}

private:

	void CreateBuffer(ID3D12Device* device, UINT byteSize)
	{
		HRESULT result = S_FALSE;

		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(byteSize);
		// バッファの生成
		result = device->CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE,
			&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&m_buffer));
		assert(SUCCEEDED(result));
	}

	void CreateView(UINT byteSize)
	{
		// GPU仮想アドレス
		m_bufferView.BufferLocation = m_buffer->GetGPUVirtualAddress();
		m_bufferView.Format = DXGI_FORMAT_R16_UINT;
		m_bufferView.SizeInBytes = byteSize;
	}

};