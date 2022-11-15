#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "TextureManager.h"
#include "SpritePipeline.h"
#include "ShapePipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstBuffer.h"

#include "MathUtility.h"


#include <DirectXMath.h>


#include "Vector3.h"

using namespace DirectX;


struct TransForm {
	Vector3 position;
	Vector3 scale;
	Vector3 angle;
	Matrix44 worldMat;

	XMFLOAT3 pos;
	XMFLOAT3 sca;
	XMFLOAT3 the;

	XMMATRIX mat;
	void CalcTransFormMatrix() {
		Matrix44 transMat = Matrix44::CreateTranslation(position);
		Matrix44 rotateMat = Matrix44::CreateRotationXYZ(angle);
		Matrix44 scaleMat = Matrix44::CreateScaling(scale);

		worldMat = scaleMat * rotateMat * transMat;

		XMMATRIX ta = XMMatrixTranslation(pos.x, pos.y, pos.z);
		XMMATRIX ro = XMMatrixRotationRollPitchYaw(the.x, the.y, the.z);
		XMMATRIX sc = XMMatrixScaling(sca.x, sca.y, sca.z);

		mat = sc * ro * ta;

	}

};


// Windows�A�v���̃G���g���[�|�C���g(main�֐�)
int MAIN
{

	WinApp * winApp = WinApp::GetInstance();
	winApp->Initialize();

	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize(winApp);

	Input* input = Input::GetInstance();
	input->Initialize(winApp);

	TextureManager* textureManager = TextureManager::GetInstance();
	textureManager->Initialize(directXCommon);

	PipelineBase* spritePipeline = SpritePipeline::GetInstance();
	spritePipeline->Create(directXCommon,D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	PipelineBase* shapePipeline = ShapePipeline::GetInstance();
	shapePipeline->Create(directXCommon,D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

	UINT testImg = 0;
	UINT aaa;
	testImg = textureManager->LoadTexture(L"resources/images/test1.png");
	aaa = textureManager->LoadTexture(L"resources/images/test.png");

	

	/*for (int i = 0; i < _countof(indices) / 3; i++) {

		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];

		Vector3 p0 = XMLoadFloat3(&vertices[index0].pos);
		Vector3 p1 = XMLoadFloat3(&vertices[index1].pos);
		Vector3 p2 = XMLoadFloat3(&vertices[index2].pos);

		Vector3 v1 = XMVectorSubtract(p1, p0);
		Vector3 v2 = XMVectorSubtract(p2, p0);

		Vector3 normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);

		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}*/

#pragma region �����̃��f��

	SpritePipeline::Vertex vertices[] =
	{	// �O
		{ {	-5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// ����
		{ { -5.0f,  5.0f,  -5.0f }, { 0.0f, 0.0f } },	// ����
		{ {  5.0f, -5.0f,  -5.0f }, { 1.0f, 1.0f } },	// �E��
		{ {  5.0f,  5.0f,  -5.0f }, { 1.0f, 0.0f } },	// �E��
		// ��
		{ {	-5.0f, -5.0f,   5.0f }, { 0.0f, 1.0f } },	// ����
		{ { -5.0f,  5.0f,   5.0f }, { 0.0f, 0.0f } },	// ����
		{ {  5.0f, -5.0f,   5.0f }, { 1.0f, 1.0f } },	// �E��
		{ {  5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// �E��
		// ��
		{ {	-5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// ����
		{ { -5.0f, -5.0f,   5.0f }, { 0.0f, 0.0f } },	// ����
		{ { -5.0f,  5.0f,  -5.0f }, { 1.0f, 1.0f } },	// �E��
		{ { -5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// �E��
		// �E
		{ {	 5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// ����
		{ {  5.0f, -5.0f,   5.0f }, { 0.0f, 0.0f } },	// ����
		{ {  5.0f,  5.0f,  -5.0f }, { 1.0f, 1.0f } },	// �E��
		{ {  5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// �E��
		// ��
		{ {	-5.0f,  5.0f,  -5.0f }, { 0.0f, 1.0f } },	// ����
		{ {  5.0f,  5.0f,  -5.0f }, { 0.0f, 0.0f } },	// ����
		{ { -5.0f,  5.0f,   5.0f }, { 1.0f, 1.0f } },	// �E��
		{ {  5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// �E��
		// ��
		{ {	-5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// ����
		{ {  5.0f, -5.0f,  -5.0f }, { 0.0f, 0.0f } },	// ����
		{ { -5.0f, -5.0f,   5.0f }, { 1.0f, 1.0f } },	// �E��
		{ {  5.0f, -5.0f,   5.0f }, { 1.0f, 0.0f } },	// �E��
	};

	uint16_t indices[] =
	{	0, 1, 2,
		2, 1, 3,

		5, 4, 6,
		5, 6, 7,

		8, 9,10,
	   10, 9,11,

	   13,12,14,
	   13,14,15,

	   17,16,18,
	   17,18,19,

	   21,22,20,
	   22,21,23
	};


	VertexBuffer<SpritePipeline::Vertex> vertBuffer;
	vertBuffer.Create(directXCommon->GetDevice(), _countof(vertices));
	vertBuffer.Map();
	std::copy(std::begin(vertices), std::end(vertices), vertBuffer.GetMapPtr());

	IndexBuffer indexBuff;
	indexBuff.Create(directXCommon->GetDevice(), _countof(indices));
	indexBuff.Map();
	std::copy(std::begin(indices), std::end(indices), indexBuff.GetMapPtr());
	indexBuff.Unmap();

#pragma endregion

#pragma region �J�����s��

	// �v���W�F�N�V�����s��
	Matrix44 projectionMat;
	float fovAngleY = Math::ToRadians(45.0f);	// �㉺��p
	float aspectRatio = (float)winApp->GetWindowWidth() / (float)winApp->GetWindowHeight();  // �A�X�y�N�g��
	float nearZ = 0.1f;			// �O�[
	float farZ = 1000.0f;		// ���[

	// �r���[�s��
	Matrix44 viewMat;
	Vector3 eye(-200 * sinf(0), 100, -200 * cosf(0));	// ���_���W
	Vector3 target(0, 0, 0);	// �����_���W
	Vector3 up(0, 1, 0);		// ������x�N�g��

	XMVECTOR xeye = XMVECTOR{ eye.x,eye.y,eye.z };
	XMVECTOR xtarget = XMVECTOR{ target.x,target.y,target.z };
	XMVECTOR xup = XMVECTOR{ up.x,up.y,up.z };

	// �������e�s��̌v�Z
	projectionMat = Matrix44::CreateProjection(fovAngleY, aspectRatio, nearZ, farZ);
	// �r���[�ϊ��s��
	viewMat = Matrix44::CreateView(eye, target, up);

	Matrix44 ort = Matrix44::CreateOrthoProjection(winApp->GetWindowWidth(), winApp->GetWindowHeight());

	XMMATRIX newproj = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
	XMMATRIX newview = XMMatrixLookAtLH(xeye, xtarget, xup);
	Matrix44 aa = viewMat * projectionMat;
	XMMATRIX a = newview * newproj;

	// ���s���e�s��̌v�Z
	//projectionMat = XMMatrixOrthographicOffCenterLH(
	//	0.0f, (float)winApp->GetWindowWidth(),
	//	(float)winApp->GetWindowHeight(), 0.0f, 0.0f, 1.0f);
	//constBuffTransform->Unmap(0, nullptr);

#pragma endregion

	float cameraAngle = 0.0f;

#pragma region �����̎���

	// �����̂̎���
	const UINT kSpriteCount = 2;

	TransForm spriteTrans[kSpriteCount];
	spriteTrans[0].scale = Vector3(5, 5, 5);
	spriteTrans[1].scale = Vector3(5, 10, 5);

	spriteTrans[1].position = Vector3(100, -50, 20);
	spriteTrans[1].angle = Vector3(0, XMConvertToRadians(45.0f), XMConvertToRadians(30.0f));

	spriteTrans[0].pos = XMFLOAT3(0, 0, 0);
	spriteTrans[0].the = XMFLOAT3(0, 0, 0);
	spriteTrans[0].sca = XMFLOAT3(5, 5, 5);

	ConstBuffer<SpritePipeline::ConstBufferData> spriteConstBuffer[kSpriteCount];
	for (auto& it : spriteConstBuffer) {
		it.Create(directXCommon->GetDevice());
		it.Map();
	}

	UINT handle[2] = { testImg, aaa };

#pragma endregion

#pragma region ��

	const int kLineVertexCount = 2;
	const int kLineNum = 3;

	float axisScale = 1000.0f;
	Vector4 xColor = { 1.0f,0.0f,0.0f,1.0f };
	Vector4 yColor = { 0.0f,1.0f,0.0f,1.0f };
	Vector4 zColor = { 0.0f,0.0f,1.0f,1.0f };
	ShapePipeline::Vertex axis[kLineNum][kLineVertexCount] =
	{	{ { {axisScale,0.0f,0.0f}, xColor },{ {-axisScale,0.0f,0.0f}, xColor } },
		{ { {0.0f,axisScale,0.0f}, yColor },{ {0.0f,-axisScale,0.0f}, yColor } },
		{ { {0.0f,0.0f,axisScale}, zColor },{ {0.0f,0.0f,-axisScale}, zColor } }  };

	ConstBuffer<ShapePipeline::ConstBufferData> axisConstBuffer; // �J�����p
	axisConstBuffer.Create(directXCommon->GetDevice());
	axisConstBuffer.Map();

	VertexBuffer<ShapePipeline::Vertex> lineVertBuffer[kLineNum];
	for (int i = 0; i < kLineNum; i++) {
		lineVertBuffer[i].Create(directXCommon->GetDevice(), kLineVertexCount);
		lineVertBuffer[i].Map();
		std::copy(std::begin(axis[i]), std::end(axis[i]), lineVertBuffer[i].GetMapPtr());
	}

#pragma endregion
	int n = 0;

	while (!winApp->WindowQUit()) {

		input->Update();
		directXCommon->PreDraw();


		// AD���͂ŃJ���������_�̎�������
		if (input->IsKeyPressed(DIK_D) || input->IsKeyPressed(DIK_A)) {
			if (input->IsKeyPressed(DIK_D)) { cameraAngle += XMConvertToRadians(1.0f); }
			else if (input->IsKeyPressed(DIK_A)) { cameraAngle -= XMConvertToRadians(1.0f); }

			eye.x = -200 * sinf(cameraAngle);
			eye.z = -200 * cosf(cameraAngle);
		}

		if (input->IsKeyPressed(DIK_W) || input->IsKeyPressed(DIK_S)) {
			if (input->IsKeyPressed(DIK_W)) { eye.y += 5.0f; }
			else if (input->IsKeyPressed(DIK_S)) { eye.y += -5.0f; }
		}

		if (input->IsKeyTrigger(DIK_SPACE)) {
			std::swap(handle[0], handle[1]);
			if (n == 0) {
				n = 1;
			}
			else {
				n = 0;
			}
		}


		if (input->IsKeyPressed(DIK_I)) {
			spriteTrans[n].position.y += 5;
		}
		if (input->IsKeyPressed(DIK_K)) {
			spriteTrans[n].position.y -= 5;
		}

		if (input->IsKeyPressed(DIK_J)) {
			spriteTrans[n].position.x -= 5;
		}
		if (input->IsKeyPressed(DIK_L)) {
			spriteTrans[n].position.x += 5;
		}

		if (input->IsKeyPressed(DIK_O)) {
			spriteTrans[n].position.z += 5;
		}
		if (input->IsKeyPressed(DIK_U)) {
			spriteTrans[n].position.z -= 5;
		}


		target = spriteTrans[n].position;

		// �������e�s��̌v�Z
		projectionMat = Matrix44::CreateProjection(fovAngleY, aspectRatio, nearZ, farZ);
		// �r���[�ϊ��s��
		viewMat = Matrix44::CreateView(eye, target, up);


		// ���`��
		axisConstBuffer.MapPtr()->mat = viewMat * projectionMat;
		for (int i = 0; i < kLineNum; i++) {

			shapePipeline->SetPipelineState(directXCommon->GetCommandList(), kBlendModeAlpha);
			directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			lineVertBuffer[i].IASet(directXCommon->GetCommandList());
			axisConstBuffer.SetGraphicsRootConstantBufferView(directXCommon->GetCommandList(), 0);
			directXCommon->GetCommandList()->DrawInstanced(kLineVertexCount, 1, 0, 0);
		}

		newproj = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
		newview = XMMatrixLookAtLH(xeye, xtarget, xup);
	
		// �X�v���C�g�`��
		for (int i = 0; i < kSpriteCount; i++) {

			spriteTrans[i].CalcTransFormMatrix();
			XMMATRIX xxx = spriteTrans[0].mat * newview * newproj;
			//spriteConstBuffer[i].MapPtr()->mat = spriteTrans[i].worldMat * viewMat * projectionMat;
			spriteConstBuffer[i].MapPtr()->mat = spriteTrans[i].mat * newview * newproj;
			spriteConstBuffer[i].MapPtr()->color = Vector4{ 1,1,1,1 };

			spritePipeline->SetPipelineState(directXCommon->GetCommandList(), kBlendModeAlpha);
			directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			vertBuffer.IASet(directXCommon->GetCommandList());
			indexBuff.IASet(directXCommon->GetCommandList());
			spriteConstBuffer[i].SetGraphicsRootConstantBufferView(directXCommon->GetCommandList(), 0);

			textureManager->SetGraphicsRootDescriptorTable(directXCommon->GetCommandList(), 1, handle[i]);

			directXCommon->GetCommandList()->DrawIndexedInstanced(indexBuff.GetIndexCount(), 1, 0, 0, 0);
		}


		


		directXCommon->PostDraw();
	}


	winApp->Finalize();

	return 0;
}



