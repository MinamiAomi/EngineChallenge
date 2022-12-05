#include "Model.h"
#include <cassert>
#include "DirectXCommon.h"
#include "TextureManager.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

DirectXCommon* Model::diXCom = nullptr;
TextureManager* Model::texMana = nullptr;
std::string Model::directory;
Model::ComPtr<ID3D12RootSignature> Model::rootSignature; // ルートシグネチャ
Model::ComPtr<ID3D12PipelineState> Model::pipelineState; // パイプラインステート



void Model::StaticInitalize(DirectXCommon* dixcom, TextureManager* texmana, const std::string& dir)
{
	assert(dixcom != nullptr);
	assert(texmana != nullptr);

	diXCom = dixcom;
	texMana = texmana;
	directory = dir;

	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob> vsBlob; // 頂点シェーダオブジェクト
	ComPtr<ID3DBlob> psBlob; // ピクセルシェーダオブジェクト
	ComPtr<ID3DBlob> errorBlob; // エラーオブジェクト

	// 頂点シェーダの読み込みとコンパイル
	result = D3DCompileFromFile(
		L"ModelVS.hlsl", // シェーダファイル名
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", // エントリーポイント名、シェーダーモデル指定
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // デバッグ用設定
		0, &vsBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG

	result = D3DCompileFromFile(
		L"ModelPS.hlsl", // シェーダファイル名
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


	// ルートシグネチャ

	// ディスクリプタレンジ
	D3D12_DESCRIPTOR_RANGE descriptorRange =
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);

	// ルートパラメータ
	CD3DX12_ROOT_PARAMETER rootParams[kRootParameterCount] = {};
	rootParams[kTransform].InitAsConstantBufferView(0);
	rootParams[kMaterial].InitAsConstantBufferView(1);
	rootParams[kTexture].InitAsDescriptorTable(1, &descriptorRange);


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

	pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBAすべてのチャンネルを描画
	pipelineDesc.BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;			// 加算
	pipelineDesc.BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;				// ソースの値を 100% 使う
	pipelineDesc.BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;			// デストの値を   0% 使う
	pipelineDesc.BlendState.RenderTarget[0].BlendEnable = true;						// ブレンドを有効にする
	pipelineDesc.BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;				// 加算
	pipelineDesc.BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;			// ソースのアルファ値
	pipelineDesc.BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-ソースのアルファ値

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
	result = diXCom->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

#pragma endregion



}

Model::Ptr Model::CreateFromObj(const std::string& path)
{
	// パスに.objが含まれている
	assert(strstr(path.c_str(), ".obj") !=  NULL);
	
	std::string filepath = directory + path;
	std::ifstream objfile(filepath);
	// ファイル が開けない場合
	assert(objfile);

	// バッファ
	std::string mtlFilePath; // mtlファイルパス
	std::vector<Vector3> posbuf; // 座標
	std::vector<Vector3> normalbuf; // 法線
	std::vector<Vector2> uvbuf; // uv
	std::unordered_map<std::string, std::vector<Meth::Vertex>> vertcies; // 頂点インデックス
	std::unordered_map<std::string, std::vector<uint16_t>> indcies; // 頂点インデックス
	Ptr result(new Model);


	std::string currentMtlName;

	std::string line;
	// ファイル終わりまで
	while (std::getline(objfile, line)) {
		// 現在読み込んでいるマテリアルの名前
		
		if (line.empty()) { // 空の行は飛ばす
			continue;
		}
		std::istringstream iss(line);	// スペースで区切る 
		std::string token;
		iss >> token; // 先頭の要素がトークンになる

		if (token == "v") {	// 頂点座標
			Vector3 tmp;
			iss >> tmp.x >> tmp.y >> tmp.z;
			posbuf.emplace_back(tmp);
		}
		else if (token == "vn") { // 法線
			Vector3 tmp;
			iss >> tmp.x >> tmp.y >> tmp.z;
			normalbuf.emplace_back(tmp);
		}
		else if (token == "vt") { // uv
			Vector2 tmp;
			iss >> tmp.x >> tmp.y;
			tmp.y = 1.0f - tmp.y; // objファイルのuv座標はy軸が反転しているため
			uvbuf.emplace_back(tmp);
		}
		else if (token == "mtllib") { // mtlファイルのパス
			iss >> mtlFilePath;
		}
		else if (token == "usemtl") { // マテリアルの設定
			iss >> currentMtlName;
			// マテリアルと対応するメッシュを生成
			result->m_materials.emplace_back(Material::Create(currentMtlName));
			result->m_meths.emplace_back(Meth::Create(currentMtlName));
			indcies[currentMtlName] = {};
			vertcies[currentMtlName] = {};
		}
		else if (token == "f") { // インデックス

			Vertex tmpV = {};
			std::string vertIndex;
			int v = 0, vn = 0, vt = 0;
			for (int i = 0; i < 3; i++) {
				iss >> vertIndex;
				sscanf(vertIndex.c_str(), "%d/%d/%d", &v, &vn, &vt);
				tmpV.position = posbuf[v - 1];
				tmpV.normal = normalbuf[vn - 1];
				tmpV.uv = uvbuf[vt - 1];
				vertcies[currentMtlName].emplace_back(tmpV);
				indcies[currentMtlName].emplace_back(vertcies.size());
			}
		}
	}




}
