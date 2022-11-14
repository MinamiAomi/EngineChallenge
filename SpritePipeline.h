#pragma once
#include "PipelineBase.h"

class SpritePipeline :
	public PipelineBase 
{

public:
	// 頂点構造体
	struct Vertex 
	{
		XMFLOAT3 pos; // 座標
		XMFLOAT2 uv;  // uv
	};

	struct ConstBufferData
	{
		XMFLOAT4 color;	// 色
		XMMATRIX mat;	// 変換行列
	};



public:
	static SpritePipeline* GetInstance();

	void Create(DirectXCommon* dixcom, D3D12_PRIMITIVE_TOPOLOGY_TYPE type);



private:
	SpritePipeline() = default;
	~SpritePipeline() = default;
	SpritePipeline(const SpritePipeline&) = delete;
	const SpritePipeline& operator=(const SpritePipeline&) = delete;

};
