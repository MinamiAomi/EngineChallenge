#pragma once
#include "PipelineBase.h"

class SpritePipeline :
	public PipelineBase 
{

public:
	// ���_�\����
	struct Vertex 
	{
		Vector3 pos; // ���W
		Vector2 uv;  // uv
	};

	struct ConstBufferData
	{
		Vector4 color;	// �F
		Matrix44 mat;	// �ϊ��s��
		//DirectX::XMMATRIX mat;
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
