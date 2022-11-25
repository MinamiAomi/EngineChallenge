//#pragma once
//#include "PipelineBase.h"
//
//class SpritePipeline :
//	public PipelineBase 
//{
//
//public:
//	// 頂点構造体
//	struct Vertex 
//	{
//		Vector3 pos; // 座標
//		Vector2 uv;  // uv
//	};
//
//	struct ConstBufferData
//	{
//		Vector4 color;	// 色
//		Matrix44 worldMat;	// 変換行列
//	};
//
//
//
//public:
//	static SpritePipeline* GetInstance();
//
//	void Create(DirectXCommon* dixcom, D3D12_PRIMITIVE_TOPOLOGY_TYPE type);
//
//
//
//private:
//	SpritePipeline() = default;
//	~SpritePipeline() = default;
//	SpritePipeline(const SpritePipeline&) = delete;
//	const SpritePipeline& operator=(const SpritePipeline&) = delete;
//
//};
