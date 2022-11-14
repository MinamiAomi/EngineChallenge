#pragma once

#include <wrl.h>
#include <Array>
#include "DirectXCommon.h"

class TextureManager 
{

private: // エイリアス
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

	struct Texture
	{
		ComPtr<ID3D12Resource> buffer; // バッファ
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle; // CPUハンドル
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle; // GPUハンドル

	};

private: // 定数
	static constexpr size_t kDescriptorCount = 50;

private: // メンバ変数
	DirectXCommon* m_directXCommon;

	ComPtr<ID3D12DescriptorHeap> m_srvHeap;
	UINT m_descriptorIncrementSize;
	std::array<Texture, kDescriptorCount> m_textures;

	UINT m_nextLoadIndex = 0;

public: // メンバ関数
	static TextureManager* GetInstance();
	static UINT LoadTexture(const wchar_t* filepath);

	void Initialize(DirectXCommon* dixCom);
	void ResetAll();

	void SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* cmdlist, UINT rootParamIndex, UINT texNumber);
private:
	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(const TextureManager&) = delete;
	const TextureManager& operator=(const TextureManager&) = delete;

	UINT LoadTexturePri(const wchar_t* filepath);
};