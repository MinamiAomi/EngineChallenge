#include "Sprite.h"
#include <cassert>
#include "d3dx12.h"
#include <d3dcompiler.h>
#include "DirectXCommon.h"
#include "TextureManager.h"
#include <vector>
#include "Camera2D.h"

#pragma comment(lib,"d3dcompiler.lib")

DirectXCommon* Sprite::diXCom = nullptr;
TextureManager* Sprite::texManager = nullptr;

Sprite::ComPtr<ID3D12RootSignature> Sprite::rootSignature; // ルートシグネチャ
Sprite::ComPtr<ID3D12PipelineState> Sprite::pipelineState[kBlendModeCount]; // パイプラインステート

IndexBuffer Sprite::indexBuffer; // インデックス

void Sprite::StaticInitalize(DirectXCommon* dixCom, TextureManager* texMan, UINT winWidth, UINT winHeight)
{
	assert(dixCom != nullptr);
	assert(texMan != nullptr);
	diXCom = dixCom;
	texManager = texMan;

	std::vector<uint16_t> indices = 
	{	0, 1, 2,
		2, 3, 0	};

	indexBuffer.Create(dixCom->GetDevice(), kIndexCount);
	indexBuffer.Map();
	std::copy(indices.begin(), indices.end(), indexBuffer.GetMapPtr());
	indexBuffer.Unmap();

#pragma region パイプライン設定

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


	// ルートシグネチャ

	// ディスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange =
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootParams[2] = {};
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

	result = diXCom->GetDevice()->CreateRootSignature(0,
		rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	

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
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // カリングしない
		//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ワイヤーフレーム

		// ブレンドステート
		SetBlendState(pipelineDesc.BlendState.RenderTarget[0], (BlendMode)i);

		// 頂点レイアウトの設定
		pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
		pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

		// 図形の形状設定
		pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

		pipelineDesc.NumRenderTargets = 1; // 描画対象は１つ
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255指定のRGBA
		pipelineDesc.SampleDesc.Count = 1; // １ピクセルに着き１回サンプリング

		pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// パイプラインにルートシグネチャをセット
		pipelineDesc.pRootSignature = rootSignature.Get();

		// パイプラインステートの生成
		result = diXCom->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState[i]));
		assert(SUCCEEDED(result));
	}

#pragma endregion

}

Sprite* Sprite::Create(uint32_t textureHandle, Vector2 position, Vector2 size, Vector4 color, Vector2 anchorPoint, bool isFlipX, bool isFlipY)
{
	return new Sprite(textureHandle, position, size, color, anchorPoint, isFlipX, isFlipY);
}

void Sprite::Draw(Sprite& sprite, const Camera2D* camera, BlendMode blend)
{

	auto* cmdList = diXCom->GetCommandList();
	// パイプラインをセット
	cmdList->SetPipelineState(pipelineState[blend].Get());
	cmdList->SetGraphicsRootSignature(rootSignature.Get());
	// 三角形リストにセット
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	// 頂点データを転送
	sprite.TransferVertex(cmdList, texManager);
	// インデックスバッファをセット
	indexBuffer.IASet(cmdList);
	// 定数を転送
	sprite.TransferConstData(cmdList, camera);
	// 画像をセット
	texManager->SetGraphicsRootDescriptorTable(cmdList, 1, sprite.GetTextureHandle());
	// 描画
	cmdList->DrawIndexedInstanced(kIndexCount, 1, 0, 0, 0);
}

Sprite::Sprite() {}

Sprite::Sprite(UINT textureHandle, Vector2 position, Vector2 size, Vector4 color, Vector2 anchorPoint, bool isFlipX, bool isFlipY) :
	textureHandle(textureHandle),
	position(position),
	color(color),
	anchorPoint(anchorPoint),
	isFlipX(isFlipX),
	isFlipY(isFlipY)
{
	vertexBuffer.Create(diXCom->GetDevice(), kVertexCount);
	vertexBuffer.Map();
	constBuffer.Create(diXCom->GetDevice());
	constBuffer.Map();
}


void Sprite::SetTextureRect(const Vector2& texBase, const Vector2& texSize)
{
	this->texBase = texBase;
	this->texSize = texSize;
}

void Sprite::TransferVertex(ID3D12GraphicsCommandList* cmdList, TextureManager* texMana)
{
	
	auto resDesc = texManager->GetRsourceDesc(textureHandle);

	float posLeft = -anchorPoint.x;
	float posRight = posLeft + size.x;
	float posTop = -anchorPoint.y;
	float posBottom = posTop + size.y;

	float uvLeft = texBase.x / static_cast<float>(resDesc.Width);
	float uvRight = texBase.x + texSize.x / static_cast<float>(resDesc.Width);
	float uvTop = texBase.y / static_cast<float>(resDesc.Height);
	float uvBottom = texBase.y + texSize.y / static_cast<float>(resDesc.Height);

	if (isFlipX == true) {
		Math::Swap(uvLeft, uvRight);
	}
	if (isFlipY == true) {
		Math::Swap(uvTop, uvBottom);
	}
	

	std::vector<Vertex> vertices;

	vertices = {
		{ { posLeft,	posBottom,	0.0f }, { uvLeft,  uvBottom } } , // 左下
		{ { posLeft,	posTop,		0.0f }, { uvLeft,  uvTop	} } , // 左上
		{ { posRight,	posTop,		0.0f }, { uvRight, uvTop	} } , // 右上
		{ { posRight,	posBottom,	0.0f }, { uvRight, uvBottom } }	  // 右下
	};

	std::copy(vertices.begin(), vertices.end(), vertexBuffer.GetMapPtr());

	vertexBuffer.IASet(cmdList);
}

void Sprite::TransferConstData(ID3D12GraphicsCommandList* cmdList, const Camera2D* camera) {
	worldMat = Matrix44::CreateRotationZ(rotation) * Matrix44::CreateTranslation({ position, 0.0f });
	constBuffer.MapPtr()->matrix = worldMat * camera->GetTransformMatrix();
	constBuffer.MapPtr()->color = color;
	constBuffer.SetGraphicsRootConstantBufferView(cmdList, 0);
}