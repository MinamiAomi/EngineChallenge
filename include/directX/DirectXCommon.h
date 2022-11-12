#pragma once
#ifndef DIRECTX_COMMON_H_
#define DIRECTX_COMMON_H_

#include "WinApp.h"
#include <wrl.h>
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>

class DirectXCommon 
{

private:
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

private:
	WinApp* m_winApp = nullptr;

	ComPtr<ID3D12Device>				 m_device;
	ComPtr<IDXGIFactory6>				 m_dxgiFactory;
	ComPtr<ID3D12CommandAllocator>		 m_cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList>	 m_cmdList;
	ComPtr<ID3D12CommandQueue>			 m_cmdQueue;
	ComPtr<IDXGISwapChain4>				 m_swapChain;
	std::vector< ComPtr<ID3D12Resource>> m_backBuffers;
	ComPtr<ID3D12Fence>					 m_fence;
	ComPtr<ID3D12DescriptorHeap>		 m_rtvHeap;

	UINT64 mFenceVal = 0;

	FLOAT clearColor[4] = { 0.1f,0.25f,0.5f,0.0f };


public: // �ÓI�����o�֐�
	static DirectXCommon* GetInstance();

public:
	inline ID3D12Device* GetDevice() { return m_device.Get(); }
	inline ID3D12GraphicsCommandList* GetCommandList() { return m_cmdList.Get(); }

	inline IDXGISwapChain4* GetSwapChain() { return m_swapChain.Get(); }
	inline ID3D12Resource* GetBackBaffer(int i) { return m_backBuffers[i].Get(); }


	void Initialize(WinApp* winApp);
	void PreDraw();
	void PostDraw();
	
	/// <summary>
	/// ���_�o�b�t�@��萔�o�b�t�@�̐���
	/// </summary>
	ComPtr<ID3D12Resource> CreateResourceBuffer(UINT64 size);

private:
	DirectXCommon() = default;
	~DirectXCommon() = default;
	DirectXCommon(const DirectXCommon&) = delete;
	const DirectXCommon& operator=(const DirectXCommon&) = delete;

	void CreateFactory();
	void CreateDevice();
	void CreateCommand();
	void CreateSwapChain();
	void CreateRenderTargetView();
	void CreateFence();

#ifdef _DEBUG
	void DebugLayer();
	void DebugSuppressError();
#endif // _DEBUG
};

#endif DIRECTX_COMMON_H_