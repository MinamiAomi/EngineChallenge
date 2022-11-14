#include "SpritePipeline.h"

#include <cassert>
#include "d3dx12.h"
#include <d3dcompiler.h>

#pragma comment(lib,"d3dcompiler.lib")

SpritePipeline* SpritePipeline::GetInstance() 
{
	static SpritePipeline instance;
	return &instance;
}

void SpritePipeline::Create(DirectXCommon* dixcom, D3D12_PRIMITIVE_TOPOLOGY_TYPE type) {

	HRESULT result = S_FALSE;

	ComPtr<ID3DBlob> vsBlob; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"SpriteVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &vsBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG

	result = D3DCompileFromFile(
		L"SpritePS.hlsl", // �V�F�[�_�t�@�C����
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
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};


	for (int i = 0; i < kBlendModeCount; i++) {

		// �O���t�B�b�N�X�p�C�v���C���ݒ�
		D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc = {};
		// �V�F�[�_�̐ݒ�
		pipelineDesc.VS = CD3DX12_SHADER_BYTECODE(vsBlob.Get());
		pipelineDesc.PS = CD3DX12_SHADER_BYTECODE(psBlob.Get());

		// �T���v���}�X�N�ݒ�
		pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
		// ���X�^���C�U�̐ݒ�
		pipelineDesc.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
		pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �J�����O���Ȃ�
		//pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME; // ���C���[�t���[��

		// �u�����h�X�e�[�g
		SetBlendState(pipelineDesc.BlendState.RenderTarget[0], (BlendMode)i);

		// ���_���C�A�E�g�̐ݒ�
		pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
		pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

		// �}�`�̌`��ݒ�
		pipelineDesc.PrimitiveTopologyType = type;

		pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ͂P��
		pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255�w���RGBA
		pipelineDesc.SampleDesc.Count = 1; // �P�s�N�Z���ɒ����P��T���v�����O

		// �f�B�X�N���v�^�����W
		D3D12_DESCRIPTOR_RANGE descriptorRange =
			CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


		CD3DX12_ROOT_PARAMETER rootParams[3] = {};
		rootParams[0].InitAsConstantBufferView(0);
		rootParams[1].InitAsDescriptorTable(1, &descriptorRange);


		// �e�N�X�`���T���v���[�̐ݒ�
		D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
		samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
		samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
		samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
		samplerDesc.MinLOD = 0.0f;
		samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
		samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

		pipelineDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
		pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;

		// ���[�g�V�O�l�`��

		// ���[�g�V�O�l�`���ݒ�
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		rootSignatureDesc.pParameters = rootParams; // ���[�g�p�����[�^�̐擪�A�h���X
		rootSignatureDesc.NumParameters = _countof(rootParams);		// ���[�g�p�����[�^�̐�
		rootSignatureDesc.pStaticSamplers = &samplerDesc;
		rootSignatureDesc.NumStaticSamplers = 1;

		// ���[�g�V�O�l�`���̃V���A���C�Y
		ComPtr<ID3DBlob> rootSigBlob;
		result = D3D12SerializeRootSignature(&rootSignatureDesc,
			D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
		assert(SUCCEEDED(result));

		result = dixcom->GetDevice()->CreateRootSignature(0,
			rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
			IID_PPV_ARGS(&m_rootSignature[i]));
		assert(SUCCEEDED(result));
		// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
		pipelineDesc.pRootSignature = m_rootSignature[i].Get();

		// �p�C�v���C���X�e�[�g�̐���
		result = dixcom->GetDevice()->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&m_pipelineState[i]));
		assert(SUCCEEDED(result));

	}
}
