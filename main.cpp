#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"

#include <d3dx12.h>
#include <cassert>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

#pragma comment(lib,"d3dcompiler.lib")

using namespace DirectX;

template<class TYPE>
using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
#endif // _DEBUG

struct ConstBufferData
{
	XMFLOAT4 color; // �F�iRGBA�j
};

enum BlendMode 
{
	kBlendModeNone,
	kBlendModeAlpha,
	kBlendModeAdd,
	kBlendModeSubtract,
	kBlendModeMultiply,
	kBlendModeInversion
};

void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode = kBlendModeAlpha);

// Windows�A�v���̃G���g���[�|�C���g(main�֐�)
int MAIN
{

	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize();

	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize(winApp);
	
	Input* input = Input::GetInstance();
	input->Initialize(winApp);

	HRESULT result = S_FALSE;

#pragma region ���_�f�[�^

	XMFLOAT3 vertices[] = 
	{	{ -0.5f, -0.5f, 0.0f },
		{ -0.5f,  0.5f, 0.0f },
		{  0.5f, -0.5f, 0.0f }	};

	ComPtr<ID3D12Resource> vertBuff;

	D3D12_HEAP_PROPERTIES vbHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC vbResourceDesc = CD3DX12_RESOURCE_DESC::Buffer(sizeof(vertices));
	// ���_�o�b�t�@�̐���
	result = directXCommon->GetDevice()->CreateCommittedResource(
		&vbHeapProp, D3D12_HEAP_FLAG_NONE,
		&vbResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));

	// GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
	XMFLOAT3* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap); // �}�b�s���O
	assert(SUCCEEDED(result));
	// ���W���R�s�[
	std::copy(std::begin(vertices), std::end(vertices), vertMap);
	vertBuff->Unmap(0, nullptr);

	// ���_�o�b�t�@�r���[�̐���
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	vbView.SizeInBytes = sizeof(vertices);
	vbView.StrideInBytes = sizeof(vertices[0]);

#pragma endregion

#pragma region �萔�f�[�^

	ComPtr<ID3D12Resource> constBuff;

	D3D12_HEAP_PROPERTIES cbHeapProp = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
	D3D12_RESOURCE_DESC cbResourceDesc = CD3DX12_RESOURCE_DESC::Buffer((sizeof(ConstBufferData) + 0xFF) & ~0xFF);
	// �萔�o�b�t�@�̐���
	result = directXCommon->GetDevice()->CreateCommittedResource(
		&cbHeapProp, D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&constBuff));
	assert(SUCCEEDED(result));

	ConstBufferData* constMap = nullptr;
	result = constBuff->Map(0, nullptr, (void**)&constMap); // �}�b�s���O
	assert(SUCCEEDED(result));
	// �l���������ނƎ����I�ɓ]�������
	constMap->color = XMFLOAT4(1.0f, 0.0f, 0.0f, 0.5f);
	constBuff->Unmap(0, nullptr);

#pragma endregion

#pragma region �V�F�[�_�ƃp�C�v���C��

	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &vsBlob, &errorBlob);
	
#ifdef _DEBUG
		ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG

	result = D3DCompileFromFile(
		L"BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &psBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG


	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
		{
			"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
	// �V�F�[�_�̐ݒ�
	pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
	pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

	// �T���v���}�X�N�ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_NONE; // �J�����O���Ȃ�
	//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ���C���[�t���[��
	
	// �u�����h�X�e�[�g
	SetBlendState(pipelineDesc.BlendState.RenderTarget[0], kBlendModeAlpha);

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ͂P��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // �P�s�N�Z���ɒ����P��T���v�����O

	CD3DX12_ROOT_PARAMETER rootParam = {};
	rootParam.InitAsConstantBufferView(0, 0, D3D12_SHADER_VISIBILITY_ALL);


	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;

	// ���[�g�V�O�l�`���ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = &rootParam; // ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = 1;		// ���[�g�p�����[�^�̐�

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, 
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = directXCommon->GetDevice()->CreateRootSignature(0,
		rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	// �p�C�v���C���X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	result = directXCommon->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));


	float x = 0.0f;
	float xspeed = 0.01f;

#pragma endregion

	while (!winApp->WindowQUit()) {

		input->Update();
		directXCommon->PreDraw();

		x += xspeed;
		if (x > 1.0f || x < 0.0f) {
			xspeed = -xspeed;
		}

		result = constBuff->Map(0, nullptr, (void**)&constMap); // �}�b�s���O
		assert(SUCCEEDED(result));
		// �l���������ނƎ����I�ɓ]�������
		constMap->color = XMFLOAT4(x, (1.0f - x), 0.0f, 0.5f);
		constBuff->Unmap(0, nullptr);
		

		directXCommon->GetCommandList()->SetPipelineState(pipelineState.Get());
		directXCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
		directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
		directXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());
		directXCommon->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0);
		

		directXCommon->PostDraw();
	}

	winApp->Finalize();

	return 0;
}


void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode) {
	blendDesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL; // RGBA���ׂẴ`�����l����`��
	blendDesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;			// ���Z
	blendDesc.SrcBlendAlpha = D3D12_BLEND_ONE;				// �\�[�X�̒l�� 100% �g��
	blendDesc.DestBlendAlpha = D3D12_BLEND_ZERO;			// �f�X�g�̒l��   0% �g��

	switch (blendMode)
	{
	case kBlendModeNone:
		blendDesc.BlendEnable = false;						// �u�����h���Ȃ�
		return;
	default:
	case kBlendModeAlpha:
		blendDesc.BlendEnable = true;						// �u�����h��L���ɂ���
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
		blendDesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;			// �\�[�X�̃A���t�@�l
		blendDesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;	// 1.0f-�\�[�X�̃A���t�@�l
		return;
	case kBlendModeAdd:
		blendDesc.BlendEnable = true;						// �u�����h��L���ɂ���
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
		blendDesc.SrcBlend = D3D12_BLEND_ONE;				// �\�[�X�̒l�� 100% �g��
		blendDesc.DestBlend = D3D12_BLEND_ONE;				// �f�X�g�̒l�� 100% �g��
		return;
	case kBlendModeSubtract:
		blendDesc.BlendEnable = true;						// �u�����h��L���ɂ���
		blendDesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;	// �f�X�g����\�[�X�����Z
		blendDesc.SrcBlend = D3D12_BLEND_ONE;				// �\�[�X�̒l�� 100% �g��
		blendDesc.DestBlend = D3D12_BLEND_ONE;				// �f�X�g�̒l�� 100% �g��
		return;
	case kBlendModeMultiply:
		blendDesc.BlendEnable = true;						// �u�����h��L���ɂ���
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
		blendDesc.SrcBlend = D3D12_BLEND_ZERO;				// �g��Ȃ�
		blendDesc.DestBlend = D3D12_BLEND_SRC_COLOR;		// �f�X�g�̒l �~ �\�[�X�̒l
		return;
	case kBlendModeInversion:
		blendDesc.BlendEnable = true;						// �u�����h��L���ɂ���
		blendDesc.BlendOp = D3D12_BLEND_OP_ADD;				// ���Z
		blendDesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;	// 1.0f-�f�X�g�J���[�̒l
		blendDesc.DestBlend = D3D12_BLEND_ZERO;				// �g��Ȃ�
		return;
	}
}

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob)
{
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());

		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
}
#endif // _DEBUG