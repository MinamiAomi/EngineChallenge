#include "TextureManager.h"

#include <cassert>
#include <DirectXTex.h>
#include <d3dx12.h>



using namespace DirectX;

TextureManager* TextureManager::GetInstance() 
{
	static TextureManager instance;
	return &instance;
}

UINT TextureManager::LoadTexture(const std::string& filePath)
{
	return GetInstance()->LoadTexturePri(filePath);
}

void TextureManager::Initialize(DirectXCommon* dixCom)
{
	m_directXCommon = dixCom;

	ResetAll();
	
	m_descriptorIncrementSize = m_directXCommon->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

void TextureManager::ResetAll() 
{
	HRESULT result = S_FALSE;
	// デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kDescriptorCount;

	result = m_directXCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&m_srvHeap));
	assert(SUCCEEDED(result));

	for (auto it : m_textures) {
		it.buffer.Reset();
		it.cpuHandle.ptr = 0;
		it.gpuHandle.ptr = 0;
	}

	m_nextLoadIndex = 0;
}

D3D12_RESOURCE_DESC TextureManager::GetRsourceDesc(UINT texHandle)
{
	assert(m_nextLoadIndex < kDescriptorCount);
	return m_textures[texHandle].buffer->GetDesc();
}

UINT TextureManager::LoadTexturePri(const std::string& filePath)
{
	assert(m_nextLoadIndex < kDescriptorCount);

	wchar_t wpath[256] = {};
	MultiByteToWideChar(CP_ACP, 0, filePath.c_str(), -1, wpath, _countof(wpath));

	HRESULT result = S_FALSE;
	
	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	result = LoadFromWICFile(wpath, WIC_FLAGS_NONE, &metadata, scratchImg);


	ScratchImage mipChain = {};

	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	metadata.format = MakeSRGB(metadata.format);


	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC texResourceDesc = {};
	texResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResourceDesc.Format = metadata.format;
	texResourceDesc.Width = metadata.width;
	texResourceDesc.Height = (UINT)metadata.height;
	texResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	texResourceDesc.SampleDesc.Count = 1;


	result = m_directXCommon->GetDevice()->CreateCommittedResource(
		&texHeapProp, D3D12_HEAP_FLAG_NONE, &texResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_textures[m_nextLoadIndex].buffer));
	assert(SUCCEEDED(result));

	// 全ミップマップについて
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// ミップマップレベルを指定してイメージを取得
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// テクスチャバッファにデータ転送
		result = m_textures[m_nextLoadIndex].buffer->WriteToSubresource(
			(UINT)i, nullptr, img->pixels,
			(UINT)img->rowPitch, (UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

	

	m_textures[m_nextLoadIndex].cpuHandle = CD3DX12_CPU_DESCRIPTOR_HANDLE(
		m_srvHeap->GetCPUDescriptorHandleForHeapStart(), m_nextLoadIndex, m_descriptorIncrementSize);
	m_textures[m_nextLoadIndex].gpuHandle = CD3DX12_GPU_DESCRIPTOR_HANDLE(
		m_srvHeap->GetGPUDescriptorHandleForHeapStart(), m_nextLoadIndex, m_descriptorIncrementSize);


	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texResourceDesc.MipLevels;

	m_directXCommon->GetDevice()->CreateShaderResourceView(m_textures[m_nextLoadIndex].buffer.Get(), &srvDesc, m_textures[m_nextLoadIndex].cpuHandle);

	return m_nextLoadIndex++;
}

void TextureManager::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* cmdlist, UINT rootParamIndex, UINT texNumber) {
	assert(texNumber < kDescriptorCount);
	assert(m_textures[texNumber].gpuHandle.ptr != 0);
	
	ID3D12DescriptorHeap* ppHeaps[] = { m_srvHeap.Get() };
	cmdlist->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	cmdlist->SetGraphicsRootDescriptorTable(rootParamIndex, m_textures[texNumber].gpuHandle);
}

// コメントアウトは自分でテクスチャデータを作るやつ

//constexpr size_t kTextureWidth = 256;
//constexpr size_t kTextureHeight = 256;
//constexpr size_t kImageDataCount = kTextureHeight * kTextureWidth;
//
//XMFLOAT4* imageData = new XMFLOAT4[kImageDataCount];


//for (size_t i = 0; i < kImageDataCount; i++) {
//	imageData[i].x = 1.0f;
//	imageData[i].y = 0.0f;
//	imageData[i].z = 0.0f;
//	imageData[i].w = 1.0f;
//}

//		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R32G32B32A32_FLOAT, kTextureWidth, kTextureHeight);
//	texResourceDesc.MipLevels = 1;

//	result = texBuff->WriteToSubresource(0, nullptr, imageData,
//		sizeof(XMFLOAT4) * kTextureWidth, sizeof(XMFLOAT4) * kImageDataCount);
//	assert(SUCCEEDED(result));


//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = 1;