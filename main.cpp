#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"

#include <d3dx12.h>
#include <cassert>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>

#include <DirectXTex.h>

#pragma comment(lib,"d3dcompiler.lib")

#include "Vector3.h"

using namespace DirectX;

template<class TYPE>
using ComPtr = Microsoft::WRL::ComPtr<TYPE>;

#ifdef _DEBUG
void ShalderFileLoadCheak(const HRESULT& result, ID3DBlob* errorBlob);
#endif // _DEBUG

struct ConstBufferDataMaterial
{
	XMFLOAT4 color; // �F�iRGBA�j
};

struct ConstBufferDataTransform
{
	XMMATRIX mat; // �RD�ϊ��s��
};

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT2 uv;

};

struct VertexPosColor {
	XMFLOAT3 pos;
	XMFLOAT4 color;
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

struct PipelineSet {
	ComPtr<ID3D12PipelineState> pipelineState;
	ComPtr<ID3D12RootSignature> rootSignature;
};

struct DrawLineDesc {
	ID3D12GraphicsCommandList* cmdList;
	PipelineSet pipeline;
	D3D12_VERTEX_BUFFER_VIEW view;
	ID3D12Resource* constBuff;
};

enum TopologyType {
	kTopologyTypeTriAngle,
	kTopologyTypeLine
};

PipelineSet CreatePrimitivePipelineSet(ID3D12Device*, D3D12_PRIMITIVE_TOPOLOGY_TYPE, BlendMode);

void SetBlendState(D3D12_RENDER_TARGET_BLEND_DESC& blendDesc, BlendMode blendMode = kBlendModeAlpha);

void SetResourceDescTexture2DMeta(D3D12_RESOURCE_DESC& texResourceDesc, const TexMetadata& metadata);

void DrawLine(DrawLineDesc&);

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

	Vertex vertices[] = 
	{	{ {	-50.0f, -50.0f,  0.0f }, { 0.0f, 1.0f } }, 
		{ { -50.5f,  50.0f,  0.0f }, { 0.0f, 0.0f } },
		{ {  50.5f, -50.0f,  0.0f }, { 1.0f, 1.0f } },
		{ {  50.5f,  50.0f,  0.0f }, { 1.0f, 0.0f } } };

	ComPtr<ID3D12Resource> vertBuff = directXCommon->CreateResourceBuffer(sizeof(vertices));

	// GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
	Vertex* vertMap = nullptr;
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

#pragma region �f�X�N���v�^�q�[�v�̐���

	// �f�X�N���v�^�q�[�v�̐���
	constexpr size_t kMaxSRVCount = 2056;

	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	ComPtr<ID3D12DescriptorHeap> srvHeap;
	result = directXCommon->GetDevice()->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(&srvHeap));
	assert(SUCCEEDED(result));

	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

#pragma endregion

#pragma region �e�N�X�`���f�[�^

	// �R�����g�A�E�g�͎����Ńe�N�X�`���f�[�^�������

	//constexpr size_t kTextureWidth = 256;
	//constexpr size_t kTextureHeight = 256;
	//constexpr size_t kImageDataCount = kTextureHeight * kTextureWidth;
	//
	//XMFLOAT4* imageData = new XMFLOAT4[kImageDataCount];

	//for (size_t i = 0; i < kImageDataCount; i++) {
	//	imageData[i].x = 1.0f;
	//	imageData[i].y = 0.0f;
	//	imageData[i].z = 0.0f;
	//	imageData[i].w = 1.0f;
	//}


	TexMetadata metadata = {};
	ScratchImage scratchImg = {};

	result = LoadFromWICFile(L"resources/images/test.png", WIC_FLAGS_NONE, &metadata, scratchImg);


	ScratchImage mipChain = {};

	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}

	metadata.format = MakeSRGB(metadata.format);


	ComPtr<ID3D12Resource> texBuff;

	D3D12_HEAP_PROPERTIES texHeapProp = {};
	texHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	texHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	texHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_RESOURCE_DESC texResourceDesc = {};
//		CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_R32G32B32A32_FLOAT, kTextureWidth, kTextureHeight);
//	texResourceDesc.MipLevels = 1;
	SetResourceDescTexture2DMeta(texResourceDesc, metadata);
	

	result = directXCommon->GetDevice()->CreateCommittedResource(
		&texHeapProp, D3D12_HEAP_FLAG_NONE, &texResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&texBuff));
	assert(SUCCEEDED(result));

	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i, nullptr, img->pixels,
			(UINT)img->rowPitch, (UINT)img->slicePitch);
		assert(SUCCEEDED(result));
	}

//	result = texBuff->WriteToSubresource(0, nullptr, imageData,
//		sizeof(XMFLOAT4) * kTextureWidth, sizeof(XMFLOAT4) * kImageDataCount);
//	assert(SUCCEEDED(result));


//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
//	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = 1;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = texResourceDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = texResourceDesc.MipLevels;

	directXCommon->GetDevice()->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);


#pragma endregion


#pragma region �C���f�b�N�X�f�[�^

	uint16_t indices[] =
	{	0, 1, 2,
		1, 2, 3   };

	ComPtr<ID3D12Resource> indexBuff = directXCommon->CreateResourceBuffer(sizeof(indices));

	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	for (int i = 0; i < _countof(indices); i++) {
		indexMap[i] = indices[i];
	}
	indexBuff->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̐���
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	// GPU���z�A�h���X
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeof(indices);

#pragma endregion

#pragma region �萔�f�[�^

	// �v���W�F�N�V�����s��
	XMMATRIX projectionMat;
	float fovAngleY = XMConvertToRadians(45.0f);	// �㉺��p
	float aspectRatio = (float)winApp->GetWindowWidth() / (float)winApp->GetWindowHeight();  // �A�X�y�N�g��
	float nearZ = 0.1f;			// �O�[
	float farZ = 1000.0f;		// ���[

	// �r���[�s��
	XMMATRIX viewMat;
	XMFLOAT3 eye(0, 100, -100);	// ���_���W
	XMFLOAT3 target(0, 0, 0);	// �����_���W
	XMFLOAT3 up(0, 1, 0);		// ������x�N�g��

	XMMATRIX worldMat;

	// ���s���e�s��̌v�Z
	//projectionMat = XMMatrixOrthographicOffCenterLH(
	//	0.0f, (float)winApp->GetWindowWidth(),
	//	(float)winApp->GetWindowHeight(), 0.0f, 0.0f, 1.0f);
	//constBuffTransform->Unmap(0, nullptr);

	// �������e�s��̌v�Z
	projectionMat = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
	// �r���[�ϊ��s��
	viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
	// ���[���h�ϊ��s��
	worldMat = XMMatrixIdentity();

	ComPtr<ID3D12Resource> constBuffMaterial = directXCommon->CreateResourceBuffer((sizeof(ConstBufferDataMaterial) + 0xFF) & ~0xFF);

	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
	assert(SUCCEEDED(result));
	// �l���������ނƎ����I�ɓ]�������
	constMapMaterial->color = XMFLOAT4(1, 0, 0, 1);
	constBuffMaterial->Unmap(0, nullptr);

	ComPtr<ID3D12Resource> constBuffTransform = directXCommon->CreateResourceBuffer((sizeof(ConstBufferDataTransform) + 0xFF) & ~0xFF);

	ConstBufferDataTransform* constMapTransform = nullptr;
	result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // �}�b�s���O
	assert(SUCCEEDED(result));
	// �l���������ނƎ����I�ɓ]�������
	constMapTransform->mat = XMMatrixIdentity();
	// �ϊ��s��̍���
	constMapTransform->mat = worldMat * viewMat * projectionMat;
	constBuffTransform->Unmap(0, nullptr);

#pragma endregion

#pragma region �e�N�X�`���p�i�{�؁j

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
		{
			"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		},
	};

#pragma region 

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

	// �f�B�X�N���v�^�����W
	D3D12_DESCRIPTOR_RANGE descriptorRange =
		CD3DX12_DESCRIPTOR_RANGE(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);


	CD3DX12_ROOT_PARAMETER rootParams[3] = {};
	rootParams[0].InitAsConstantBufferView(0);
	rootParams[1].InitAsDescriptorTable(1, &descriptorRange);
	rootParams[2].InitAsConstantBufferView(1);

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

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;

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

#pragma endregion


	float x = 0.0f;
	float xspeed = 0.01f;

	float cameraAngle = 0.0f;

#pragma endregion

	const int kLineVertexCount = 2;
	const int kLineNum = 3;

	UINT sizeLineVB = static_cast<UINT>(sizeof(VertexPosColor) * kLineVertexCount);
	
	ComPtr<ID3D12Resource> lineVertBuff[3];
	VertexPosColor* lineVertMap[3] = { nullptr };
	D3D12_VERTEX_BUFFER_VIEW lineVbView[3] = {};

	for (int i = 0; i < kLineNum; i++) {

		lineVertBuff[i] = directXCommon->CreateResourceBuffer(sizeLineVB);

		lineVertMap[i] = nullptr;

		// GPU��̃o�b�t�@�ɑΉ��������z�������i���C����������j���擾
		result = lineVertBuff[i]->Map(0, nullptr, (void**)&lineVertMap[i]); // �}�b�s���O
		assert(SUCCEEDED(result));


		// GPU���z�A�h���X
		lineVbView[i].BufferLocation = lineVertBuff[i]->GetGPUVirtualAddress();
		lineVbView[i].SizeInBytes = sizeLineVB;
		lineVbView[i].StrideInBytes = sizeof(VertexPosColor);

	}

	PipelineSet linePipeline = CreatePrimitivePipelineSet(
		directXCommon->GetDevice(), D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE, kBlendModeAlpha);

	DrawLineDesc drawLineDesc;
	drawLineDesc.cmdList = directXCommon->GetCommandList();
	drawLineDesc.pipeline = linePipeline;
	drawLineDesc.constBuff = constBuffTransform.Get();

	float axisScale = 1000.0f;
	XMFLOAT4 xColor = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 yColor = { 0.0f,1.0f,0.0f,1.0f };
	XMFLOAT4 zColor = { 0.0f,0.0f,1.0f,1.0f };
	VertexPosColor xAxis[2] = { {{axisScale,0.0f,0.0f},xColor},{{-axisScale,0.0f,0.0f},xColor} };
	VertexPosColor yAxis[2] = { {{0.0f,axisScale,0.0f},yColor},{{0.0f,-axisScale,0.0f},yColor} };
	VertexPosColor zAxis[2] = { {{0.0f,0.0f,axisScale},zColor},{{0.0f,0.0f,-axisScale},zColor} };

	while (!winApp->WindowQUit()) {

		input->Update();
		directXCommon->PreDraw();

		// AD���͂ŃJ���������_�̎�������
		if (input->IsKeyPressed(DIK_D) || input->IsKeyPressed(DIK_A)) {
			if(input->IsKeyPressed(DIK_D)){ cameraAngle += XMConvertToRadians(1.0f); }
			else if(input->IsKeyPressed(DIK_A)) { cameraAngle -= XMConvertToRadians(1.0f); }

			eye.x = -200 * sinf(cameraAngle);
			eye.z = -200 * cosf(cameraAngle);
		}
		
		// �������e�s��̌v�Z
		projectionMat = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);

		// �r���[�ϊ��s��
		viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		result = constBuffTransform->Map(0, nullptr, (void**)&constMapTransform); // �}�b�s���O
		assert(SUCCEEDED(result));
		// �l���������ނƎ����I�ɓ]�������
		constMapTransform->mat = XMMatrixIdentity();
		// �ϊ��s��̍���
		constMapTransform->mat = worldMat * viewMat * projectionMat;
		constBuffTransform->Unmap(0, nullptr);



		x += xspeed;
		if (x > 1.0f || x < 0.0f) {
			xspeed = -xspeed;
		}

		result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
		assert(SUCCEEDED(result));
		// �l���������ނƎ����I�ɓ]�������
		constMapMaterial->color = XMFLOAT4(x, (1.0f - x), 0.0f, 0.5f);
		constBuffMaterial->Unmap(0, nullptr);
		

		std::copy(std::begin(xAxis), std::end(xAxis), lineVertMap[0]);
		drawLineDesc.view = lineVbView[0];
		DrawLine(drawLineDesc);
		
		std::copy(std::begin(yAxis), std::end(yAxis), lineVertMap[1]);
		drawLineDesc.view = lineVbView[1];
		DrawLine(drawLineDesc);
		
		std::copy(std::begin(zAxis), std::end(zAxis), lineVertMap[2]);
		drawLineDesc.view = lineVbView[2];
		DrawLine(drawLineDesc);

		
		directXCommon->GetCommandList()->SetPipelineState(pipelineState.Get());
		directXCommon->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
		directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		directXCommon->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
		directXCommon->GetCommandList()->IASetIndexBuffer(&ibView);
		directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(0, constBuffMaterial->GetGPUVirtualAddress());
		
		directXCommon->GetCommandList()->SetDescriptorHeaps(1, srvHeap.GetAddressOf());
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		directXCommon->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		
		directXCommon->GetCommandList()->SetGraphicsRootConstantBufferView(2, constBuffTransform->GetGPUVirtualAddress());
		
		directXCommon->GetCommandList()->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);
		

		directXCommon->PostDraw();
	}


	winApp->Finalize();

	return 0;
}


void DrawLine(DrawLineDesc& desc) {
	desc.cmdList->SetPipelineState(desc.pipeline.pipelineState.Get());
	desc.cmdList->SetGraphicsRootSignature(desc.pipeline.rootSignature.Get());
	desc.cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	desc.cmdList->IASetVertexBuffers(0, 1, &desc.view);
	desc.cmdList->SetGraphicsRootConstantBufferView(0, desc.constBuff->GetGPUVirtualAddress());
	desc.cmdList->DrawInstanced(2, 1, 0, 0);
}

PipelineSet CreatePrimitivePipelineSet(ID3D12Device* device, D3D12_PRIMITIVE_TOPOLOGY_TYPE type, BlendMode blendmode)
{
	
	PipelineSet tmp;

	HRESULT result = S_FALSE;
	
	ComPtr<ID3DBlob> vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ComPtr<ID3DBlob> errorBlob = nullptr; // �G���[�I�u�W�F�N�g

	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"ShapeVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE,
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0, &vsBlob, &errorBlob);

#ifdef _DEBUG
	ShalderFileLoadCheak(result, errorBlob.Get());
#endif // _DEBUG

	result = D3DCompileFromFile(
		L"ShapePS.hlsl", // �V�F�[�_�t�@�C����
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
			"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,
			D3D12_APPEND_ALIGNED_ELEMENT,
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
		}
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
	SetBlendState(pipelineDesc.BlendState.RenderTarget[0], blendmode);

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = type;

	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ͂P��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0 ~ 255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // �P�s�N�Z���ɒ����P��T���v�����O

	

	CD3DX12_ROOT_PARAMETER rootParams[1] = {};
	rootParams[0].InitAsConstantBufferView(0);

	// ���[�g�V�O�l�`��
	
	// �X�^�e�B�b�N�T���v���[
	CD3DX12_STATIC_SAMPLER_DESC samplerDesc = CD3DX12_STATIC_SAMPLER_DESC(0);

	// ���[�g�V�O�l�`���ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; // ���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);		// ���[�g�p�����[�^�̐�
	rootSignatureDesc.NumStaticSamplers = 1;
	rootSignatureDesc.pStaticSamplers = &samplerDesc;

	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob;
	result = D3D12SerializeRootSignature(&rootSignatureDesc,
		D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));

	result = device->CreateRootSignature(0,
		rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&tmp.rootSignature));
	assert(SUCCEEDED(result));
	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = tmp.rootSignature.Get();

	// �p�C�v���C���X�e�[�g�̐���
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&tmp.pipelineState));
	assert(SUCCEEDED(result));

	return tmp;
}


void SetResourceDescTexture2DMeta(D3D12_RESOURCE_DESC& texResourceDesc, const TexMetadata& metadata) {
	texResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texResourceDesc.Format = metadata.format;
	texResourceDesc.Width = metadata.width;
	texResourceDesc.Height = (UINT)metadata.height;
	texResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	texResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	texResourceDesc.SampleDesc.Count = 1;
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