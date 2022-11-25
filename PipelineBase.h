//#pragma once
//#include "DirectXCommon.h"
//#include "MathUtility.h"
//
//
//class PipelineBase 
//{
//
//protected:
//	template<class TYPE>
//	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;
//
//public:
//	struct CommonConstData
//	{
//		Matrix44 cameraMat;
//	};
//
//protected:
//	ComPtr<ID3D12PipelineState> pipelineState;
//	ComPtr<ID3D12RootSignature> rootSignature;
//
//public:
//	virtual void SetPipelineState(ID3D12GraphicsCommandList* cmdlist);
//	
//protected:
//	PipelineBase() {}
//	virtual ~PipelineBase() = 0;
//
//#ifdef _DEBUG
//	void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
//#endif // _DEBUG
//
//};