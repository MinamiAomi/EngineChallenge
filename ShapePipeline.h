#pragma once
#include "PipelineBase.h"

class ShapePipeline :
	public PipelineBase
{

public:
	// ���_�\����
	struct Vertex
	{
		Vector3 pos; // ���W
		Vector4 color;
	};

	struct ConstBufferData
	{
		Matrix44 worldMat;	// �ϊ��s��
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
