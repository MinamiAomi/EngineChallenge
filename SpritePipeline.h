#pragma once
#include "PipelineBase.h"

class SpritePipeline :
	public PipelineBase 
{

public:
	// ���_�\����
	struct Vertex 
	{
		XMFLOAT3 pos; // ���W
		XMFLOAT2 uv;  // uv
	};

	struct ConstBufferData
	{
		XMFLOAT4 color;	// �F
		XMMATRIX mat;	// �ϊ��s��
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
