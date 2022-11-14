#pragma once
#include "DirectXCommon.h"
#include "MathUtility.h"

enum BlendMode
{
	kBlendModeNone,			// ブレンドなし
	kBlendModeAlpha,		// アルファブレンド
	kBlendModeAdd,			// 加算合成
	kBlendModeSubtract,		// 減算合成
	kBlendModeMultiply,		// 乗算合成
	kBlendModeInversion,	// 色反転

	kBlendModeCount
};


class PipelineBase 
{

protected:
	template<class TYPE>
	using ComPtr = Microsoft::WRL::ComPtr<TYPE>;


protected:
	ComPtr<ID3D12PipelineState> m_pipelineState[kBlendModeCount];
	ComPtr<ID3D12RootSignature> m_rootSignature[kBlendModeCount];

public:
	virtual void Create(DirectXCommon* dixcom, D3D12_PRIMITIVE_TOPOLOGY_TYPE type) = 0;

	virtual void SetPipelineState(ID3D12GraphicsCommandList* cmdlist, BlendMode blendmode);
	
protected:
	void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendmode);

	PipelineBase() = default;
	virtual ~PipelineBase() = default;
	PipelineBase(const PipelineBase&) = delete;
	const PipelineBase& operator=(const PipelineBase&) = delete;

#ifdef _DEBUG
	void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
#endif // _DEBUG

};