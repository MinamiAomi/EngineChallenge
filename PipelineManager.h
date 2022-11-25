#pragma once
//#include <vector>
//#include "PipelineBase.h"
//#include <memory>
//#include "DirectXCommon.h"
//
//enum PipelineType {
//	kPipelineTypeShape,
//	kPipelineTypeSprite,
//
//	kPipelineTypeCount
//};
//
//
//class PipelineManager
//{
//
//private:
//	struct PipelineData
//	{
//		std::unique_ptr<PipelineBase> pipeline;
//		D3D12_PRIMITIVE_TOPOLOGY_TYPE type;
//		BlendMode blendmode;
//	};
//
//private:
//	DirectXCommon* diXCom;
//
//	std::vector<PipelineData> pipelines;
//	UINT nextHandle = 0;
//
//public:
//	static PipelineManager* GetInstance();
//
//	void Initalize(DirectXCommon* dixCom);
//	UINT CreatePipeline(PipelineType plType, D3D12_PRIMITIVE_TOPOLOGY_TYPE type, BlendMode blendmode);
//	void SetPipelineState(UINT pipelineHandle);
//
//private:
//	PipelineManager() = default;
//	virtual ~PipelineManager() = default;
//	PipelineManager(const PipelineManager&) = delete;
//	const PipelineManager& operator=(const PipelineManager&) = delete;
//};

