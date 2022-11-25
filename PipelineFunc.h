#pragma once
#include "DirectXCommon.h"
#include "MathUtility.h"

enum BlendMode
{
	//kBlendModeNone,			// ブレンドなし
	kBlendModeAlpha,		// アルファブレンド
	//kBlendModeAdd,			// 加算合成
	//kBlendModeSubtract,		// 減算合成
	//kBlendModeMultiply,		// 乗算合成
	//kBlendModeInversion,	// 色反転

	kBlendModeCount
};

void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendmode);

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
#endif // _DEBUG