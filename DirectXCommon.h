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
	WinApp* m_winApp;

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

public: // ê√ìIÉÅÉìÉoä÷êî
	static DirectXCommon* GetInstance();

public:
	void Initialize(WinApp* winApp);
	void Finalize();

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