#include "SpritePipeline.h"

#include <cassert>
#include "d3dx12.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

SpritePipeline* SpritePipeline::GetInstance() 
{
	static SpritePipeline instance;
	return &instance;
}

void SpritePipeline::Create(DirectXCommon* dixcom, D3D12_PRIMITIVE_TOPOLOGY_TYPE type) {

	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"SpriteVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &vsBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG

	result = D3DCompileFromFile(
		L"SpritePS.hlsl", // シェーダファイル名
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
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};


	for (int i = 0; i < kBlendModeCount; i++) {

		// グラフィックスパイプライン設定
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
		// シェーダの設定
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

		// サンプルマスク設定
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		// ラスタライザの設定
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // カリングしない
		//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ワイヤーフレーム

		// ブレンドステート
		SetBlendState(pipelineDesc.BlendState.RenderTarget[0], (BlendMode)i);

		// 頂点レイアウトの設定
		pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
		pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

		// 図形の形状設定
		pipelineDesc.PrimitiveTopologyType = type;

		pipelineDesc.NumRenderTargets = 1; // 描画対象は１つ
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255指定のRGBA
		pipelineDesc.SampleDesc.Count = 1; // １ピクセルに着き１回サンプリング

		// ディスクリプタレンジ
		D3D12_DESCRIPTOR_RANGE descriptorRange =
			CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


		CD3DX12_ROOT_PARAMETER rootParams[3] = {};
		rootParams[0].InitAsConstantBufferView(0);
		rootParams[1].InitAsDescriptorTable(1, &descriptorRange);


		// テクスチャサンプラーの設定
		D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// ルートシグネチャ

		// ルートシグネチャ設定
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.pParameters = rootParams; // ルートパラメータの先頭アドレス
		rootSignatureDesc.NumParameters = _countof(rootParams);		// ルートパラメータの数
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.NumStaticSamplers = 1;

		// ルートシグネチャのシリアライズ
		ComPtr<ID3DBlob> rootSigBlob;
		result = D3D12SerializeRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		assert(SUCCEEDED(result));

		result = dixcom->GetDevice()->CreateRootSignature(0,
			rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&m_rootSignature[i]));
		assert(SUCCEEDED(result));
		// パイプラインにルートシグネチャをセット
		pipelineDesc.pRootSignature = m_rootSignature[i].Get();

		// パイプラインステートの生成
		result = dixcom->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&m_pipelineState[i]));
		assert(SUCCEEDED(result));

	}
}
