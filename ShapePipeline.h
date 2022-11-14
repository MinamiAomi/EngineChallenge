#pragma once
#include "PipelineBase.h"

class ShapePipeline :
	public PipelineBase
{

public:
	// 頂点構造体
	struct Vertex
	{
		XMFLOAT3 pos; // 座標
		XMFLOAT4 color;
	};

	struct ConstBufferData
	{
		XMMATRIX mat;	// 変換行列
	};

public:
	static ShapePipeline* GetInstance();

	void Create(DirectXCommon* dixcom, D3D12_PRIMITIVE_TOPOLOGY_TYPE type);



private:
	ShapePipeline() = default;
	~ShapePipeline() = default;
	ShapePipeline(const ShapePipeline&) = delete;
	const ShapePipeline& operator=(const ShapePipeline&) = delete;

};
