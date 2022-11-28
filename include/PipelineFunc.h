#pragma once
#include "DirectXCommon.h"
#include "MathUtility.h"
#include <string>

enum BlendMode
{
	//kBlendModeNone,			// ƒuƒŒƒ“ƒh‚È‚µ
	kBlendModeNormal,		// ’Êí
	//kBlendModeAdd,			// ‰ÁZ‡¬
	//kBlendModeSubtract,		// Œ¸Z‡¬
	//kBlendModeMultiply,		// æZ‡¬
	//kBlendModeInversion,	// F”½“]

	kBlendModeCount
};

void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendmode);

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
#endif // _DEBUG