//#include "PipelineBase.h"
//
//#include <string>
//#include <cassert>
//
//void PipelineBase::SetPipelineState(ID3D12GraphicsCommandList* cmdlist) 
//{
//	cmdlist->SetPipelineState(pipelineState.Get());
//	cmdlist->SetGraphicsRootSignature(rootSignature.Get());
//}
//
//
//#ifdef _DEBUG
//void PipelineBase::ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob)
//{
//	if (FAILED(result)) {
//		// errorBlob����G���[���e��string�^�ɃR�s�[
//		std::string error;
//		error.resize(errorBlob->GetBufferSize());
//
//		std::copy_n((char*)errorBlob->GetBufferPointer(),
//			errorBlob->GetBufferSize(),
//			error.begin());
//		error += "\n";
//		// �G���[���e���o�̓E�B���h�E�ɕ\��
//		OutputDebugStringA(error.c_str());
//		assert(0);
//	}
//}
//#endif // _DEBUG