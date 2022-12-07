#pragma once
#include <vector>
#include <memory>
#include <string>
#include <wrl.h>
#include <array>
#include <unordered_map>

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "MathUtility.h"
#include "PipelineFunc.h"
#include "Meth.h"
#include "Material.h"

class Model 
{
private:
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

public:
	struct Vertex
	{
		Vector3 position;	// 座標
		Vector3 normal;		// 法線
		Vector2 uv;			// uv座標
	};


	enum RootParameter 
	{
		kWorldTransform,			// ワールド行列
		kCamera,		// カメラデータ（行列も含む）
		kMaterial,		// マテリアル
		kTexture,       // テクスチャ

		kRootParameterCount
	};

private:

	static class DirectXCommon* diXCom;
	static class TextureManager* texMana;
	static std::string directory;
	static ComPtr<ID3D12RootSignature> rootSignature; // ルートシグネチャ
	static ComPtr<ID3D12PipelineState> pipelineState; // パイプラインステート
	static std::unique_ptr<Material> defaultMaterial;

public:

	/// <summary>
	/// 静的メンバの初期化
	/// </summary>
	/// <param name="dixcom">DirectXCommonのポインタ</param>
	/// <param name="dir">モデルのディレクトリ</param>
	static void StaticInitalize(DirectXCommon* dixcom, TextureManager* texmana, const std::string& dir = "resources/model/");

	/// <summary>
	/// objファイルから生成
	/// </summary>
	/// <param name="path">ファイルパス</param>
	/// <returns>Modelのスマートポインタ</returns>
	static std::unique_ptr<Model> CreateFromObj(const std::string& path);

private:
	
	std::string m_name;
	std::vector<std::unique_ptr<Meth>> m_meths;
	std::unordered_map<std::string, std::unique_ptr<Material>> m_materials;

public:
	~Model() {}

	void Draw(ID3D12GraphicsCommandList* cmdList, class Object3D* object);
private:
	Model() {}
};
