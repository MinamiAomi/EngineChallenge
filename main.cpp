#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "TextureManager.h"

#include <d3dx12.h>
#include <cassert>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

#include <DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")

#include "Vector3.h"

using namespace DirectX;

template<class TYPE>
using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
#endif // _DEBUG

struct ConstBufferDataMaterial
{
	XMFLOAT4 color; // 色（RGBA）
};

struct ConstBufferDataTransform
{
	XMMATRIX mat; // ３D変換行列
};

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT3 normal;
	XMFLOAT2 uv;

};

struct VertexPosColor {
	XMFLOAT3 pos;
	XMFLOAT4 color;
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

struct PipelineSet {
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;
};

struct DrawLineDesc {
	ID3D12GraphicsCommandList* cmdList;
	PipelineSet pipeline;
	D3D12_VERTEX_BUFFER_VIEW view;
	ID3D12Resource* constBuff;
};

enum TopologyType {
	kTopologyTypeTriAngle,
	kTopologyTypeLine
};

PipelineSet CreatePrimitivePipelineSet(ID3D12Device*, D3D12_PRIMITIVE_TOPOLOGY_TYPE, BlendMode);

void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode = kBlendModeAlpha);



void DrawLine(DrawLineDesc&);

// Windowsアプリのエントリーポイント(main関数)
int MAIN
{

	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize();

	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize(winApp);
	
	Input* input = Input::GetInstance();
	input->Initialize(winApp);

	TextureManager* textureManager = TextureManager::GetInstance();
	textureManager->Initialize(directXCommon);

	UINT testImg = 0;
	UINT aaa;
	testImg = textureManager->LoadTexture(L"resources/images/test1.png");
	aaa = textureManager->LoadTexture(L"resources/images/test.png");

	HRESULT result = S_FALSE;

	Vertex vertices[] =
	{	// 前
		{ {	-5.0f, -5.0f,  -5.0f },{}, { 0.0f, 1.0f } },	// 左下
		{ { -5.0f,  5.0f,  -5.0f },{}, { 0.0f, 0.0f } },	// 左上
		{ {  5.0f, -5.0f,  -5.0f },{}, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f,  5.0f,  -5.0f },{}, { 1.0f, 0.0f } },	// 右上
		// 後
		{ {	-5.0f, -5.0f,   5.0f },{}, { 0.0f, 1.0f } },	// 左下
		{ { -5.0f,  5.0f,   5.0f },{}, { 0.0f, 0.0f } },	// 左上
		{ {  5.0f, -5.0f,   5.0f },{}, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f,  5.0f,   5.0f },{}, { 1.0f, 0.0f } },	// 右上
		// 左
		{ {	-5.0f, -5.0f,  -5.0f },{}, { 0.0f, 1.0f } },	// 左下
		{ { -5.0f, -5.0f,   5.0f },{}, { 0.0f, 0.0f } },	// 左上
		{ { -5.0f,  5.0f,  -5.0f },{}, { 1.0f, 1.0f } },	// 右下
		{ { -5.0f,  5.0f,   5.0f },{}, { 1.0f, 0.0f } },	// 右上
		// 右
		{ {	 5.0f, -5.0f,  -5.0f },{}, { 0.0f, 1.0f } },	// 左下
		{ {  5.0f, -5.0f,   5.0f },{}, { 0.0f, 0.0f } },	// 左上
		{ {  5.0f,  5.0f,  -5.0f },{}, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f,  5.0f,   5.0f },{}, { 1.0f, 0.0f } },	// 右上
		// 上
		{ {	-5.0f,  5.0f,  -5.0f },{}, { 0.0f, 1.0f } },	// 左下
		{ {  5.0f,  5.0f,  -5.0f },{}, { 0.0f, 0.0f } },	// 左上
		{ { -5.0f,  5.0f,   5.0f },{}, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f,  5.0f,   5.0f },{}, { 1.0f, 0.0f } },	// 右上
		// 下
		{ {	-5.0f, -5.0f,  -5.0f },{}, { 0.0f, 1.0f } },	// 左下
		{ {  5.0f, -5.0f,  -5.0f },{}, { 0.0f, 0.0f } },	// 左上
		{ { -5.0f, -5.0f,   5.0f },{}, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f, -5.0f,   5.0f },{}, { 1.0f, 0.0f } },	// 右上
	};

	uint16_t indices[] =
	{   0, 1, 2,
		2, 1, 3,

		5, 4, 6,
		5, 6, 7,

		8, 9,10,
	   10, 9,11,

	   13,12,14,
	   13,14,15,

	   17,16,18,
	   17,18,19,

	   21,22,20,
	   22,21,23
	};

	for (int i = 0; i < _countof(indices) / 3; i++) {
	
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
	
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
	
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);
	
		XMVECTOR normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);
	
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}

#pragma region 頂点データ

	ComPtr<ID3D12Resource> vertBuff = directXCommon->CreateResourceBuffer(sizeof(vertices));

	// GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
	Vertex* vertMap = nullptr;
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

#pragma region インデックスデータ

	ComPtr<ID3D12Resource> indexBuff = directXCommon->CreateResourceBuffer(sizeof(indices));

	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	indexBuff->Unmap(0, nullptr);

	// インデックスバッファビューの生成
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// GPU仮想アドレス
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

#pragma endregion



#pragma region 定数データ

	// プロジェクション行列
	XMMATRIX projectionMat;
	float fovAngleY = XMConvertToRadians(45.0f);	// 上下画角
	float aspectRatio = (float)winApp->GetWindowWidth() / (float)winApp->GetWindowHeight();  // アスペクト比
	float nearZ = 0.1f;			// 前端
	float farZ = 1000.0f;		// 奥端

	// ビュー行列
	XMMATRIX viewMat;
	XMFLOAT3 eye(-200 * sinf(0), 100, -200 * cosf(0));	// 視点座標
	XMFLOAT3 target(0, 0, 0);	// 注視点座標
	XMFLOAT3 up(0, 1, 0);		// 上方向ベクトル
	

	XMFLOAT3 position(0, 0, 0);
	XMFLOAT3 scale(5, 5, 5);
	XMFLOAT3 angle(0, 0, 0);
	XMMATRIX worldMat;

	XMFLOAT3 position2(100, 0, 0);
	XMFLOAT3 scale2(5, 3, 5);
	XMFLOAT3 angle2(0, 2, 0);
	XMMATRIX worldMat2;

	// 並行投影行列の計算
	//projectionMat = XMMatrixOrthographicOffCenterLH(
	//	0.0f, (float)winApp->GetWindowWidth(),
	//	(float)winApp->GetWindowHeight(), 0.0f, 0.0f, 1.0f);
	//constBuffTransform->Unmap(0, nullptr);

	// 透視投影行列の計算
	projectionMat = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
	// ビュー変換行列
	viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	// ワールド変換行列
	worldMat = XMMatrixIdentity();


	ComPtr<ID3D12Resource> constBuffMaterial = directXCommon->CreateResourceBuffer((sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF);

	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
	assert(SUCCEEDED(result));
	// 値を書き込むと自動的に転送される
	constMapMaterial->color = XMFLOAT4(1, 1, 1, 1);
	constBuffMaterial->Unmap(0, nullptr);


	ComPtr<ID3D12Resource> constBuffTransform = directXCommon->CreateResourceBuffer((sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF);

	ConstBufferDataTransform* constMapTransform = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // マッピング
	assert(SUCCEEDED(result));
	// 値を書き込むと自動的に転送される
	constMapTransform->mat = XMMatrixIdentity();
	// 変換行列の合成
	constMapTransform->mat = worldMat * viewMat * projectionMat;
	constBuffTransform->Unmap(0, nullptr);

	ComPtr<ID3D12Resource> constBuffTransform2 = directXCommon->CreateResourceBuffer((sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF);

	ConstBufferDataTransform* constMapTransform2 = nullptr;
	result = constBuffTransform2->Map(0, nullptr, (void**)&constMapTransform2); // マッピング
	assert(SUCCEEDED(result));
	// 値を書き込むと自動的に転送される
	constMapTransform2->mat = XMMatrixIdentity();
	// 変換行列の合成
	constMapTransform2->mat = worldMat2 * viewMat * projectionMat;
	constBuffTransform2->Unmap(0, nullptr);

#pragma endregion

#pragma region テクスチャ用（本筋）

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
		{
			"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

#pragma region 

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
	SetBlendState(pipelineDesc.BlendState.RenderTarget[0], kBlendModeAlpha);

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = 1; // 描画対象は１つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // １ピクセルに着き１回サンプリング

	// ディスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange =
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


	CD3DX12_ROOT_PARAMETER rootParams[3] = {};
	rootParams[0].InitAsConstantBufferView(0);
	rootParams[1].InitAsDescriptorTable(1, &descriptorRange);
	rootParams[2].InitAsConstantBufferView(1);


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
	ComPtr<ID3D12RootSignature> rootSignature;

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

#pragma endregion


#pragma endregion

	float x = 0.0f;
	float xspeed = 0.01f;

	float cameraAngle = 0.0f;

	const int kLineVertexCount = 2;
	const int kLineNum = 3;

	UINT sizeLineVB = static_cast<UINT>(sizeof(VertexPosColor) * kLineVertexCount);
	
	ComPtr<ID3D12Resource> lineVertBuff[3];
	VertexPosColor* lineVertMap[3] = { nullptr };
	D3D12_VERTEX_BUFFER_VIEW lineVbView[3] = {};

	for (int i = 0; i < kLineNum; i++) {

		lineVertBuff[i] = directXCommon->CreateResourceBuffer(sizeLineVB);

		lineVertMap[i] = nullptr;

		// GPU上のバッファに対応した仮想メモリ（メインメモリ上）を取得
		result = lineVertBuff[i]->Map(0, nullptr, (void**)&lineVertMap[i]); // マッピング
		assert(SUCCEEDED(result));


		// GPU仮想アドレス
		lineVbView[i].BufferLocation = lineVertBuff[i]->GetGPUVirtualAddress();
		lineVbView[i].SizeInBytes = sizeLineVB;
		lineVbView[i].StrideInBytes = sizeof(VertexPosColor);

	}

	PipelineSet linePipeline = CreatePrimitivePipelineSet(
		directXCommon->GetDevice(), D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAlpha);

	DrawLineDesc drawLineDesc;
	drawLineDesc.cmdList = directXCommon->GetCommandList();
	drawLineDesc.pipeline = linePipeline;
	drawLineDesc.constBuff = constBuffTransform.Get();

	float axisScale = 1000.0f;
	XMFLOAT4 xColor = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 yColor = { 0.0f,1.0f,0.0f,1.0f };
	XMFLOAT4 zColor = { 0.0f,0.0f,1.0f,1.0f };
	VertexPosColor xAxis[2] = { {{axisScale,0.0f,0.0f},xColor},{{-axisScale,0.0f,0.0f},xColor} };
	VertexPosColor yAxis[2] = { {{0.0f,axisScale,0.0f},yColor},{{0.0f,-axisScale,0.0f},yColor} };
	VertexPosColor zAxis[2] = { {{0.0f,0.0f,axisScale},zColor},{{0.0f,0.0f,-axisScale},zColor} };

	while (!winApp->WindowQUit()) {

		input->Update();
		directXCommon->PreDraw();


		// AD入力でカメラが原点の周りを回る
		if (input->IsKeyPressed(DIK_D) || input->IsKeyPressed(DIK_A)) {
			if (input->IsKeyPressed(DIK_D)) { cameraAngle += XMConvertToRadians(1.0f); }
			else if (input->IsKeyPressed(DIK_A)) { cameraAngle -= XMConvertToRadians(1.0f); }

			eye.x = -200 * sinf(cameraAngle);
			eye.z = -200 * cosf(cameraAngle);
		}

		if (input->IsKeyPressed(DIK_W) || input->IsKeyPressed(DIK_S)) {
			if (input->IsKeyPressed(DIK_W)) { eye.y += 5.0f; }
			else if (input->IsKeyPressed(DIK_S)) { eye.y += -5.0f; }
		}
		
	//	x += xspeed;
	//	if (x > 1.0f || x < 0.0f) {
	//		xspeed = -xspeed;
	//	}
	//
	//	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // マッピング
	//	assert(SUCCEEDED(result));
	//	// 値を書き込むと自動的に転送される
	//	constMapMaterial->color = XMFLOAT4(x, (1.0f - x), 0.0f, 1.0f);
	//	constBuffMaterial->Unmap(0, nullptr);


		XMMATRIX transMat = XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX rotateMat = XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
		XMMATRIX scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);

		worldMat = scaleMat * rotateMat * transMat;

		// 透視投影行列の計算
		projectionMat = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);

		// ビュー変換行列
		viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // マッピング
		assert(SUCCEEDED(result));
		// 値を書き込むと自動的に転送される
		constMapTransform->mat = XMMatrixIdentity();
		// 変換行列の合成
		constMapTransform->mat = worldMat * viewMat * projectionMat;
		constBuffTransform->Unmap(0, nullptr);

		XMMATRIX transMat2 = XMMatrixTranslation(position2.x, position2.y, position2.z);
		XMMATRIX rotateMat2 = XMMatrixRotationRollPitchYaw(angle2.x, angle2.y, angle2.z);
		XMMATRIX scaleMat2 = XMMatrixScaling(scale2.x, scale2.y, scale2.z);
		worldMat2 = scaleMat2 * rotateMat2 * transMat2;

		result = constBuffTransform2->Map(0, nullptr, (void**)&constMapTransform2); // マッピング
		assert(SUCCEEDED(result));
		// 値を書き込むと自動的に転送される
		constMapTransform2->mat = XMMatrixIdentity();
		// 変換行列の合成
		constMapTransform2->mat = worldMat2 * viewMat * projectionMat;
		constBuffTransform2->Unmap(0, nullptr);


		std::copy(std::begin(xAxis), std::end(xAxis), lineVertMap[0]);
		drawLineDesc.view = lineVbView[0];
		DrawLine(drawLineDesc);
		
		std::copy(std::begin(yAxis), std::end(yAxis), lineVertMap[1]);
		drawLineDesc.view = lineVbView[1];
		DrawLine(drawLineDesc);
		
		std::copy(std::begin(zAxis), std::end(zAxis), lineVertMap[2]);
		drawLineDesc.view = lineVbView[2];
		DrawLine(drawLineDesc);


	
		
		directXCommon->GetCommandList()->SetPipelineState(pipelineState.Get());
		directXCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
		directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		directXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		directXCommon->GetCommandList()->IASetIndexBuffer(&ibView);
		directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		
		textureManager->SetGraphicsRootDescriptorTable(directXCommon->GetCommandList(), 1, testImg);
		
		directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
		
		directXCommon->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
		


		directXCommon->GetCommandList()->SetPipelineState(pipelineState.Get());
		directXCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
		directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		directXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		directXCommon->GetCommandList()->IASetIndexBuffer(&ibView);
		directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());

		textureManager->SetGraphicsRootDescriptorTable(directXCommon->GetCommandList(), 1, aaa);

		directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform2->GetGPUVirtualAddress());

		directXCommon->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

		directXCommon->PostDraw();
	}


	winApp->Finalize();

	return 0;
}


void DrawLine(DrawLineDesc& desc) {
	desc.cmdList->SetPipelineState(desc.pipeline.pipelineState.Get());
	desc.cmdList->SetGraphicsRootSignature(desc.pipeline.rootSignature.Get());
	desc.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	desc.cmdList->IASetVertexBuffers(0, 1, &desc.view);
	desc.cmdList->SetGraphicsRootConstantBufferView(0, desc.constBuff->GetGPUVirtualAddress());
	desc.cmdList->DrawInstanced(2, 1, 0, 0);
}

PipelineSet CreatePrimitivePipelineSet(ID3D12Device* device, D3D12_PRIMITIVE_TOPOLOGY_TYPE type, BlendMode blendmode)
{
	
	PipelineSet tmp;

	HRESULT result = S_FALSE;
	
	ComPtr<ID3DBlob> vsBlob = nullptr; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob = nullptr; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob = nullptr; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"ShapeVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &vsBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG

	result = D3DCompileFromFile(
		L"ShapePS.hlsl", // シェーダファイル名
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
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
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
	SetBlendState(pipelineDesc.BlendState.RenderTarget[0], blendmode);

	// 頂点レイアウトの設定
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// 図形の形状設定
	pipelineDesc.PrimitiveTopologyType = type;

	pipelineDesc.NumRenderTargets = 1; // 描画対象は１つ
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255指定のRGBA
	pipelineDesc.SampleDesc.Count = 1; // １ピクセルに着き１回サンプリング

	pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

	CD3DX12_ROOT_PARAMETER rootParams[1] = {};
	rootParams[0].InitAsConstantBufferView(0);

	// ルートシグネチャ
	
	// スタティックサンプラー
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ルートシグネチャ設定
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; // ルートパラメータの先頭アドレス
	rootSignatureDesc.NumParameters = _countof(rootParams);		// ルートパラメータの数
	rootSignatureDesc.NumStaticSamplers = 1;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;

	// ルートシグネチャのシリアライズ
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = device->CreateRootSignature(0,
		rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&tmp.rootSignature));
	assert(SUCCEEDED(result));
	// パイプラインにルートシグネチャをセット
	pipelineDesc.pRootSignature = tmp.rootSignature.Get();

	// パイプラインステートの生成
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&tmp.pipelineState));
	assert(SUCCEEDED(result));

	return tmp;
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