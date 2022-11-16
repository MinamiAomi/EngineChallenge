#pragma once
#include "PipelineBase.h"

class ShapePipeline :
	public PipelineBase
{

public:
	// 頂点構造体
	struct Vertex
	{
		Vector3 pos; // 座標
		Vector4 color;
	};

	struct ConstBufferData
	{
		Matrix44 worldMat;	// 変換行列
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
