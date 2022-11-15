#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "TextureManager.h"
#include "SpritePipeline.h"
#include "ShapePipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstBuffer.h"




#include <DirectXMath.h>


#include "Vector3.h"

using namespace DirectX;


struct TransForm {
	XMFLOAT3 position;
	XMFLOAT3 scale;
	XMFLOAT3 angle;
	XMMATRIX worldMat;

	TransForm() {
		position = XMFLOAT3{ 0,0,0 };
		scale = XMFLOAT3{ 0,0,0 };
		angle = XMFLOAT3{ 0,0,0 };
		worldMat = XMMatrixIdentity();
	}

	void CalcTransFormMatrix() {
		XMMATRIX transMat = XMMatrixTranslation(position.x, position.y, position.z);
		XMMATRIX rotateMat = XMMatrixRotationRollPitchYaw(angle.x, angle.y, angle.z);
		XMMATRIX scaleMat = XMMatrixScaling(scale.x, scale.y, scale.z);

		worldMat = scaleMat * rotateMat * transMat;
	}
};

XMFLOAT3 operator-(const XMFLOAT3& a, const XMFLOAT3& b) {
	return { a.x - b.x,a.y - b.y,a.z - b.z };
}

float length(const XMFLOAT3& a) {
	return sqrtf(a.x * a.x + a.y * a.y + a.z * a.z);
}

XMFLOAT3 normalize(const XMFLOAT3& a) {
	float len = length(a);
	return { a.x / len, a.y / len, a.z / len };
}



// Windowsアプリのエントリーポイント(main関数)
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

		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);

		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		XMVECTOR normal = XMVector3Cross(v1, v2);
		normal = XMVector3Normalize(normal);

		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);
	}*/

#pragma region 立方体モデル

	SpritePipeline::Vertex vertices[] =
	{	// 前
		{ {	-5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
		{ { -5.0f,  5.0f,  -5.0f }, { 0.0f, 0.0f } },	// 左上
		{ {  5.0f, -5.0f,  -5.0f }, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f,  5.0f,  -5.0f }, { 1.0f, 0.0f } },	// 右上
		// 後
		{ {	-5.0f, -5.0f,   5.0f }, { 0.0f, 1.0f } },	// 左下
		{ { -5.0f,  5.0f,   5.0f }, { 0.0f, 0.0f } },	// 左上
		{ {  5.0f, -5.0f,   5.0f }, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
		// 左
		{ {	-5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
		{ { -5.0f, -5.0f,   5.0f }, { 0.0f, 0.0f } },	// 左上
		{ { -5.0f,  5.0f,  -5.0f }, { 1.0f, 1.0f } },	// 右下
		{ { -5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
		// 右
		{ {	 5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
		{ {  5.0f, -5.0f,   5.0f }, { 0.0f, 0.0f } },	// 左上
		{ {  5.0f,  5.0f,  -5.0f }, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
		// 上
		{ {	-5.0f,  5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
		{ {  5.0f,  5.0f,  -5.0f }, { 0.0f, 0.0f } },	// 左上
		{ { -5.0f,  5.0f,   5.0f }, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
		// 下
		{ {	-5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
		{ {  5.0f, -5.0f,  -5.0f }, { 0.0f, 0.0f } },	// 左上
		{ { -5.0f, -5.0f,   5.0f }, { 1.0f, 1.0f } },	// 右下
		{ {  5.0f, -5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
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

#pragma region カメラ行列

	// プロジェクション行列
	XMMATRIX projectionMat;
	float fovAngleY = XMConvertToRadians(45.0f);	// 上下画角
	float aspectRatio = (float)winApp->GetWindowWidth() / (float)winApp->GetWindowHeight();  // アスペクト比
	float nearZ = 0.1f;			// 前端
	float farZ = 1000.0f;		// 奥端

	// ビュー行列
	XMMATRIX viewMat;
	XMFLOAT3 eye(-200 * sinf(0), 100, -200 * cosf(0));	// 視点座標
	XMFLOAT3 target(0, 0, 0);	// 注視点座標
	XMFLOAT3 up(0, 1, 0);		// 上方向ベクトル

	// 透視投影行列の計算
	projectionMat = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);
	// ビュー変換行列
	viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));


	// 並行投影行列の計算
	//projectionMat = XMMatrixOrthographicOffCenterLH(
	//	0.0f, (float)winApp->GetWindowWidth(),
	//	(float)winApp->GetWindowHeight(), 0.0f, 0.0f, 1.0f);
	//constBuffTransform->Unmap(0, nullptr);

#pragma endregion

	float cameraAngle = 0.0f;

#pragma region 立方体実態

	// 立方体の実態
	const UINT kSpriteCount = 2;

	TransForm spriteTrans[kSpriteCount];
	spriteTrans[0].scale = XMFLOAT3(5, 5, 5);
	spriteTrans[1].scale = XMFLOAT3(5, 10, 5);

	spriteTrans[1].position = XMFLOAT3(100, -50, 20);
	spriteTrans[1].angle = XMFLOAT3(0, XMConvertToRadians(45.0f), XMConvertToRadians(30.0f));

	ConstBuffer<SpritePipeline::ConstBufferData> spriteConstBuffer[kSpriteCount];
	for (auto& it : spriteConstBuffer) {
		it.Create(directXCommon->GetDevice());
		it.Map();
	}

	UINT handle[2] = { testImg, aaa };

#pragma endregion


#pragma region 軸

	const int kLineVertexCount = 2;
	const int kLineNum = 3;

	float axisScale = 1000.0f;
	XMFLOAT4 xColor = { 1.0f,0.0f,0.0f,1.0f };
	XMFLOAT4 yColor = { 0.0f,1.0f,0.0f,1.0f };
	XMFLOAT4 zColor = { 0.0f,0.0f,1.0f,1.0f };
	ShapePipeline::Vertex axis[kLineNum][kLineVertexCount] =
	{	{ { {axisScale,0.0f,0.0f}, xColor },{ {-axisScale,0.0f,0.0f}, xColor } },
		{ { {0.0f,axisScale,0.0f}, yColor },{ {0.0f,-axisScale,0.0f}, yColor } },
		{ { {0.0f,0.0f,axisScale}, zColor },{ {0.0f,0.0f,-axisScale}, zColor } }  };

	ConstBuffer<ShapePipeline::ConstBufferData> axisConstBuffer; // カメラ用
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


		// AD入力でカメラが原点の周りを回る
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
			target = spriteTrans[n].position;
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



		// ビュー変換行列
		viewMat = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
		// 透視投影行列の計算
		projectionMat = XMMatrixPerspectiveFovLH(fovAngleY, aspectRatio, nearZ, farZ);


		// 軸描画
		axisConstBuffer.MapPtr()->mat = viewMat * projectionMat;
		for (int i = 0; i < kLineNum; i++) {

			shapePipeline->SetPipelineState(directXCommon->GetCommandList(), kBlendModeAlpha);
			directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
			lineVertBuffer[i].IASet(directXCommon->GetCommandList());
			axisConstBuffer.SetGraphicsRootConstantBufferView(directXCommon->GetCommandList(), 0);
			directXCommon->GetCommandList()->DrawInstanced(kLineVertexCount, 1, 0, 0);
		}


	
		// スプライト描画
		for (int i = 0; i < kSpriteCount; i++) {

			spriteTrans[i].CalcTransFormMatrix();

			spriteConstBuffer[i].MapPtr()->mat = spriteTrans[i].worldMat * viewMat * projectionMat;
			spriteConstBuffer[i].MapPtr()->color = XMFLOAT4{ 1,1,1,1 };

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



