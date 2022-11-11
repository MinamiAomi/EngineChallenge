#include "DirectXCommon.h"
#include <cassert>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

DirectXCommon* DirectXCommon::GetInstance() 
{
	static DirectXCommon instance;
	return &instance;
}

void DirectXCommon::Initialize(WinApp* winApp) 
{
	m_winApp = winApp;

#ifdef _DEBUG
	DebugLayer();
#endif // _DEBUG

	CreateFactory();
	CreateDevice();

#ifdef _DEBUG
	DebugSuppressError();
#endif // _DEBUG

	CreateCommand();
	CreateSwapChain();
	CreateRenderTargetView();
	CreateFence();
}

void DirectXCommon::Finalize() 
{

}

void DirectXCommon::CreateFactory()
{
	HRESULT result = S_FALSE;
	// DXGIファクトリーの生成
	result = CreateDXGIFactory(IID_PPV_ARGS(&m_dxgiFactory));
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateDevice()
{
	HRESULT result = S_FALSE;
	// アダプターの列挙用
	std::vector<ComPtr<IDXGIAdapter4>> adapters;
	// ここに特定の名前を持つアダプターオブジェクトが入る
	ComPtr<IDXGIAdapter4> tmpAdapter = nullptr;

	// パフォーマンスが高いものから順に、すべてのアダプターを列挙する
	for (UINT i = 0;
		m_dxgiFactory->EnumAdapterByGpuPreference(
			i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND;
		i++) {
		adapters.push_back(tmpAdapter); // 配列に追加する
	}

	// 妥当なアダプタを選別する
	for (auto& it : adapters) {
		DXGI_ADAPTER_DESC3 adapterDesc;
		// アダプターの情報を取得する
		it->GetDesc3(&adapterDesc);
		// ソフトウェアデバイスを回避
		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
			tmpAdapter = it;
			break;
		}
	}

	// 対応レベル配列
	D3D_FEATURE_LEVEL levels[] =
	{ D3D_FEATURE_LEVEL_12_1,
		D3D_FEATURE_LEVEL_12_0,
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0, };

	D3D_FEATURE_LEVEL featureLevel;
	for (auto& it : levels) {
		result = D3D12CreateDevice(tmpAdapter.Get(), it, IID_PPV_ARGS(&m_device));
		if (result == S_OK) {
			featureLevel = it;
			break;
		}
	}

}

void DirectXCommon::CreateCommand()
{
	HRESULT result = S_FALSE;
	// コマンドアロケータを生成
	result = m_device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cmdAllocator));
	assert(SUCCEEDED(result));

	// コマンドリストを生成
	result = m_device->CreateCommandList(
		0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_cmdAllocator.Get(), nullptr, IID_PPV_ARGS(&m_cmdList));
	assert(SUCCEEDED(result));

	// コマンドキューの設定
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	// コマンドキューを生成
	result = m_device->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&m_cmdQueue));
	assert(SUCCEEDED(result));

}

void DirectXCommon::CreateSwapChain()
{
	HRESULT result = S_FALSE;
	// スワップチェーンの設定
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};

	swapChainDesc.Width = m_winApp->GetWindowWidth();
	swapChainDesc.Height = m_winApp->GetWindowHeight();
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // 色情報の書式
	swapChainDesc.SampleDesc.Count = 1; // マルチサンプルしない
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // バックバッファ用
	swapChainDesc.BufferCount = 2; // バッファ数を２つ設定
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	ComPtr<IDXGISwapChain1> swapChain1;

	result = m_dxgiFactory->CreateSwapChainForHwnd(
		m_cmdQueue.Get(), m_winApp->GetHwnd(), &swapChainDesc, 
		nullptr, nullptr, &swapChain1);
	assert(SUCCEEDED(result));

	// 生成したIDXGISwapChain1のオブジェクトをIDXGISwapChain4に変換する
	result = swapChain1.As(&m_swapChain);
	assert(SUCCEEDED(result));
}

void DirectXCommon::CreateRenderTargetView()
{
	HRESULT result = S_FALSE;

	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	result = m_swapChain->GetDesc(&swapChainDesc);
	assert(SUCCEEDED(result));

	// デスクリプタヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // レンダーターゲットビューなので RTV
	rtvHeapDesc.NodeMask = 0;
	rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // 表裏の２つ
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE; // 特に指定なし

	result = m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
	assert(SUCCEEDED(result));


	m_backBuffers.resize(swapChainDesc.BufferCount);
	// スワップチェーンのすべてのバッファについて処理する
	for (size_t i = 0; i < m_backBuffers.size(); i++) {
		// スワップチェーンからバッファを取得
		result = m_swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&m_backBuffers[i]));
		assert(SUCCEEDED(result));
		// デスクリプタヒープのハンドルを取得
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = m_rtvHeap->GetCPUDescriptorHandleForHeapStart();
		// 裏か表かでアドレスがずれる
		rtvHandle.ptr += i * m_device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		// レンダーターゲットビューの設定
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
		// シェーダーの計算結果をSRGBに変換して書き込む
		rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
		// レンダーターゲットビューの生成
		m_device->CreateRenderTargetView(m_backBuffers[i].Get(), &rtvDesc, rtvHandle);
	}
}

void DirectXCommon::CreateFence()
{
	HRESULT result = S_FALSE;
	result = m_device->CreateFence(mFenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	assert(SUCCEEDED(result));
}


#ifdef _DEBUG
void DirectXCommon::DebugLayer() 
{
	ComPtr<ID3D12Debug1> _debugController;
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&_debugController)))) {
		_debugController->EnableDebugLayer();
		_debugController->SetEnableGPUBasedValidation(TRUE);
	}
}
void DirectXCommon::DebugSuppressError() 
{
	ComPtr<ID3D12InfoQueue> _infoQueue;
	if (SUCCEEDED(m_device->QueryInterface(IID_PPV_ARGS(&_infoQueue)))) {
		D3D12_MESSAGE_ID denyIds[] = {
			/*
			 * Windows11でのDXGIデバッグレイヤーとDX12デバッグレイヤーの相互作用バグによるエラーメッセージ
			 * https://stackoverflow.com/questions/69805245/directx-12-application-is-crashing-in-windows-11
			 */
			 D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		// 抑制する表示レベル
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter = {};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		// 指定したエラーの表示を抑制する
		_infoQueue->PushStorageFilter(&filter);
		// エラー時にブレークを発生させる
		_infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, TRUE);
		_infoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, TRUE);
	}
}
#endif // _DEBUG