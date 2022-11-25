#pragma once
#include "DirectXCommon.h"
#include "MathUtility.h"

enum BlendMode
{
	//kBlendModeNone,			// �u�����h�Ȃ�
	kBlendModeAlpha,		// �A���t�@�u�����h
	//kBlendModeAdd,			// ���Z����
	//kBlendModeSubtract,		// ���Z����
	//kBlendModeMultiply,		// ��Z����
	//kBlendModeInversion,	// �F���]

	kBlendModeCount
};

void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendmode);

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
#endif // _DEBUG