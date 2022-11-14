#pragma once

#include <wrl.h>
#include <Array>
#include "DirectXCommon.h"

class TextureManager 
{

private: // �G�C���A�X
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

	struct Texture
	{
		ComPtr<ID3D12Resource> buffer; // �o�b�t�@
		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle; // CPU�n���h��
		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle; // GPU�n���h��

	};

private: // �萔
	static constexpr size_t kDescriptorCount = 50;

private: // �����o�ϐ�
	DirectXCommon* m_directXCommon;

	ComPtr<ID3D12DescriptorHeap> m_srvHeap;
	UINT m_descriptorIncrementSize;
	std::array<Texture, kDescriptorCount> m_textures;

	UINT m_nextLoadIndex = 0;

public: // �����o�֐�
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