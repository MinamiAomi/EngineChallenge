#include "PipelineFunc.h"
#include <string>
#include <cassert>


void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendmode)
{
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBAすべてのチャンネルを描画
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			// 加算
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;				// ソースの値を 100% 使う
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			// デストの値を   0% 使う

	switch (blendmode)
	{
		//case kBlendModeNone:
		//	blendDesc.BlendEnable = false;						// ブレンドしない
		//	return;
		//default:
	case kBlendModeNormal:
		blendDesc.BlendEnable = true;						// ブレンドを有効にする
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			// ソースのアルファ値
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-ソースのアルファ値
		return;
		//case kBlendModeAdd:
		//	blendDesc.BlendEnable = true;						// ブレンドを有効にする
		//	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		//	blendDesc.SrcBlend = D3D12_BLEND_ONE;				// ソースの値を 100% 使う
		//	blendDesc.DestBlend = D3D12_BLEND_ONE;				// デストの値を 100% 使う
		//	return;
		//case kBlendModeSubtract:
		//	blendDesc.BlendEnable = true;						// ブレンドを有効にする
		//	blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	// デストからソースを減算
		//	blendDesc.SrcBlend = D3D12_BLEND_ONE;				// ソースの値を 100% 使う
		//	blendDesc.DestBlend = D3D12_BLEND_ONE;				// デストの値を 100% 使う
		//	return;
		//case kBlendModeMultiply:
		//	blendDesc.BlendEnable = true;						// ブレンドを有効にする
		//	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		//	blendDesc.SrcBlend = D3D12_BLEND_ZERO;				// 使わない
		//	blendDesc.DestBlend = D3D12_BLEND_SRC_COLOR;		// デストの値 × ソースの値
		//	return;
		//case kBlendModeInversion:
		//	blendDesc.BlendEnable = true;						// ブレンドを有効にする
		//	blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// 加算
		//	blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f-デストカラーの値
		//	blendDesc.DestBlend = D3D12_BLEND_ZERO;				// 使わない
		//	return;
	}
}


#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob)
{
	if (FAILED(result)) {
		// errorBlobからエラー内容をstring型にコピー
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// エラー内容を出力ウィンドウに表示
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}
#endif // _DEBUG
