#pragma once
#include "MathUtility.h"
#include <wrl.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstBuffer.h"
#include "PipelineFunc.h"
#include <memory>

class Sprite
{

private:
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

public:
	// 頂点構造体
	struct Vertex
	{
		Vector3 pos; // 座標
		Vector2 uv;  // uv
	};
	// 定数
	struct ConstBufferData
	{
		Vector4 color;	// 色
		Matrix44 matrix;	// 変換行列
	};
private:
	static const int kVertexCount = 4; // 頂点数
	static const int kIndexCount = 6;
	static DirectXCommon* diXCom;
	static class TextureManager* texManager;

	static ComPtr<ID3D12RootSignature> rootSignature; // ルートシグネチャ
	static ComPtr<ID3D12PipelineState> pipelineState[kBlendModeCount]; // パイプラインステート
	
	static IndexBuffer indexBuffer; // インデックス

public: // 静的メンバ関数
	// 静的メンバの初期化
	static void StaticInitalize(DirectXCommon* dixCom, TextureManager* texMan, UINT winWidth, UINT winHeight);

	static std::unique_ptr<Sprite> Create(
		UINT  textureHandle, Vector2 position, Vector2 size = { 0,0 }, Vector4 color = { 1,1,1,1 },
		Vector2 anchorPoint = { 0,0 }, bool isFlipX = false, bool isFlipY = false);

	static void Draw(Sprite& sprite, const class Camera2D* camera, BlendMode blend);

private:
	VertexBuffer<Vertex> vertexBuffer; // 頂点バッファ
	ConstBuffer<ConstBufferData> constBuffer; // 定数バッファ
	UINT  textureHandle = 0; // テクスチャハンドル
	float rotation = 0.0f; // Z軸回りの回転角（2D回転）
	Vector2 position = { 0.0f,0.0f }; // 座標
	Vector2 size = { 100.0f,100.0f }; // サイズ
	Vector2 anchorPoint = { 0.0f,0.0f }; // 回転移動を行う基準点
	Matrix44 worldMat; // ワールド行列
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f }; // 色
	bool isFlipX = false; // 左右反転を行うか
	bool isFlipY = false; // 上下反転を行うか

	Vector2 texBase = { 0.0f,0.0f }; // 画像の左上座標
	Vector2 texSize = { 16.0f,16.0f };;// 画像の幅、高さ

public:
	Sprite();
	Sprite(UINT  textureHandle, Vector2 position, Vector2 size, Vector4 color = { 1,1,1,1 },
		Vector2 anchorPoint = { 0,0 }, bool isFlipX = false, bool isFlipY = false);

	void SetTextureHandle(UINT handle) { textureHandle = handle; }
	UINT GetTextureHandle() const { return textureHandle; }
	void SetRotation(float theta) { rotation = theta; }
	float GetRotation() const { return rotation; }
	void SetPosition(const Vector2& pos) { position = pos; }
	const Vector2& GetPosition() const { return position; }
	void SetSize(const Vector2& size) {this->size = size; }
	const Vector2& GetSize() const { return size; }
	void SetAnchorPoint(const Vector2& point) { anchorPoint = point; }
	const Vector2& GetAnchorPoint() const { return anchorPoint; }
	void SetColor(const Vector4& color) { this->color = color; }
	const Vector4& GetColor() const { return color; }
	void SetIsFlipX(bool isFlipX) { this->isFlipX = isFlipX; }
	bool GetIsFlipX() const { return isFlipX; }
	void SetIsFlipY(bool isFlipY) { this->isFlipY = isFlipY; }
	bool GetIsFlipY() const { return isFlipY; }
	void SetTextureRect(const Vector2& texBase, const Vector2& texSize);

private:
	void TransferVertex(ID3D12GraphicsCommandList* cmdList, TextureManager* texMana);
	void TransferConstData(ID3D12GraphicsCommandList* cmdList, const class Camera2D* camera);
};

