#pragma once

#include <d3d12.h>
#include "d3dx12.h"
#include <cassert>
#include <wrl.h>

template<class CONSTDATA_TYPE> class ConstBuffer
{
private:
	Microsoft::WRL::ComPtr<ID3D12Resource> m_buffer;
	CONSTDATA_TYPE* m_map = nullptr;

public:
	ConstBuffer() {}
	~ConstBuffer() {}

	void Create(ID3D12Device* device)
	{
		CreateBuffer(device);
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

	CONSTDATA_TYPE* MapPtr() { return m_map; }

	void SetGraphicsRootConstantBufferView(ID3D12GraphicsCommandList* cmdList, UINT rootParamIndex)
	{
		cmdList->SetGraphicsRootConstantBufferView(rootParamIndex, m_buffer->GetGPUVirtualAddress());
	}

private:

	void CreateBuffer(ID3D12Device* device)
	{
		HRESULT result = S_FALSE;

		D3D12_HEAP_PROPERTIES heapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		D3D12_RESOURCE_DESC resourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(CONSTDATA_TYPE) + 0xFF) & ~0xFF);
		// バッファの生成
		result = device->CreateCommittedResource(
			&heapProp, D3D12_HEAP_FLAG_NONE,
			&resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr, IID_PPV_ARGS(&m_buffer));
		assert(SUCCEEDED(result));
	}


};