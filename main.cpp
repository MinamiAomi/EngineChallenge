#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"

#include <d3dx12.h>
#include <cassert>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

template<class TYPE>
using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
#endif // _DEBUG

struct ConstBufferData
{
	XMFLOAT4 color; // 色（RGBA）
};

enum BlendMode 
{
	kBlendModeNone,
	kBlendModeAlpha,
	kBlendModeAdd,
	kBlendModeSubtract,
	kBlendModeMultiply,
	kBlendModeInversion
};

void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode = kBlendModeAlpha);

// Windowsアプリのエントリーポイント(main関数)
int MAIN
{

	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize();

	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize(winApp);
	
	Input* input = Input::GetInstance();
	input->Initialize(winApp);

	HRESULT result = S_FALSE;

#pragma region 頂点データ

	XMFLOAT3 vertices[] = 
	{	{ -0.5f, -0.5f, 0.0f },
		{ -0.5f,  0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f }	};

	ComPtr<ID3D12Resource> vertBuff;

	D3D12_HEAP_PROPERTIES vbHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC vbResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));
	// 頂点バッファの生成
	result = directXCommon->GetDevice()->CreateCommittedResource(
		&vbHeapProp, D3D12_HEAP_FLAG_NONE,
		&vbResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap); // マッピング
	assert(SUCCEEDED(result));
	// 座標をコピー
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	vertBuff->Unmap(0, nullptr);

	// 頂点バッファビューの生成
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion

#pragma region 定数データ

	ComPtr<ID3D12Resource> constBuff;

	D3D12_HEAP_PROPERTIES cbHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC cbResourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xFF) & ~0xFF);
	// 定数バッファの生成
	result = directXCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp, D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap); // マッピング
	assert(SUCCEEDED(result));
	// 値を書き込むと自動的に転送される
	constMap->color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f);
	constBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region シェーダとパイプライン

	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &vsBlob, &errorBlob);
	
#ifdef _DEBUG
		ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG

	result = D3DCompileFromFile(
		L"BasicPS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &psBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG


	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// グラフィックスパイプライン設定
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
	// シェーダの設定
	pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// サンプルマスク設定
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ラスタライザの設定
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ワイヤーフレーム
	
	// ブレンドステート
	SetBlendState(pipelineDesc.BlendState.RenderTarget[0], kBlendModeAlpha);

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = 1; // 描画対象は１つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // １ピクセルに着き１回サンプリング

	CD3DX12_ROOT_PARAMETER rootParam = {};
	rootParam.InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);


	// ルートシグネチャ
	ComPtr<ID3D12RootSignature> rootSignature;

	// ルートシグネチャ設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParam; // ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = 1;		// ルートパラメータの数

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, 
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = directXCommon->GetDevice()->CreateRootSignature(0,
		rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = rootSignature.Get();

	// パイプラインステートの生成
	ComPtr<ID3D12PipelineState> pipelineState;
	result = directXCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));


	float x = 0.0f;
	float xspeed = 0.01f;

#pragma endregion

	while (!winApp->WindowQUit()) {

		input->Update();
		directXCommon->PreDraw();

		x += xspeed;
		if (x > 1.0f || x < 0.0f) {
			xspeed = -xspeed;
		}

		result = constBuff->Map(0, nullptr, (void**)&constMap); // マッピング
		assert(SUCCEEDED(result));
		// 値を書き込むと自動的に転送される
		constMap->color = XMFLOAT4(x, (1.0f - x), 0.0f, 0.5f);
		constBuff->Unmap(0, nullptr);
		

		directXCommon->GetCommandList()->SetPipelineState(pipelineState.Get());
		directXCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
		directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		directXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
		directXCommon->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
		

		directXCommon->PostDraw();
	}

	winApp->Finalize();

	return 0;
}


void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode) {
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBAすべてのチャンネルを描画
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			// 加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;				// ソースの値を 100% 使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			// デストの値を   0% 使う

	switch (blendMode)
	{
	case kBlendModeNone:
		blendDesc.BlendEnable = false;						// ブレンドしない
		return;
	default:
	case kBlendModeAlpha:
		blendDesc.BlendEnable = true;						// ブレンドを有効にする
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			// ソースのアルファ値
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-ソースのアルファ値
		return;
	case kBlendModeAdd:
		blendDesc.BlendEnable = true;						// ブレンドを有効にする
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		blendDesc.SrcBlend = D3D12_BLEND_ONE;				// ソースの値を 100% 使う
		blendDesc.DestBlend = D3D12_BLEND_ONE;				// デストの値を 100% 使う
		return;
	case kBlendModeSubtract:
		blendDesc.BlendEnable = true;						// ブレンドを有効にする
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	// デストからソースを減算
		blendDesc.SrcBlend = D3D12_BLEND_ONE;				// ソースの値を 100% 使う
		blendDesc.DestBlend = D3D12_BLEND_ONE;				// デストの値を 100% 使う
		return;
	case kBlendModeMultiply:
		blendDesc.BlendEnable = true;						// ブレンドを有効にする
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		blendDesc.SrcBlend = D3D12_BLEND_ZERO;				// 使わない
		blendDesc.DestBlend = D3D12_BLEND_SRC_COLOR;		// デストの値 × ソースの値
		return;
	case kBlendModeInversion:
		blendDesc.BlendEnable = true;						// ブレンドを有効にする
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f-デストカラーの値
		blendDesc.DestBlend = D3D12_BLEND_ZERO;				// 使わない
		return;
	}
}

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob)
{
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}
#endif // _DEBUG