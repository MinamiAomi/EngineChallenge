#include "WinApp.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "TextureManager.h"
//#include "SpritePipeline.h"
//#include "ShapePipeline.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstBuffer.h"
#include "MathUtility.h"

#include "Camera2D.h"
#include "Sprite.h"

struct TransForm {
	Vector3 position;
	Vector3 scale;
	Vector3 angle;
	Matrix44 worldMat;

	void CalcTransFormMatrix() {
		Matrix44 transMat = Matrix44::CreateTranslation(position);
		Matrix44 rotateMat = Matrix44::CreateRotationXYZ(angle);
		Matrix44 scaleMat = Matrix44::CreateScaling(scale);

		worldMat = scaleMat * rotateMat * transMat;
	}

};

//struct LineMeth
//{
//	struct LineBuffer
//	{
//		VertexBuffer<ShapePipeline::Vertex> vertBuff;
//		ConstBuffer<ShapePipeline::ConstBufferData> worldMatBuff;
//	};
//
//	static constexpr UINT kLineMethMaxNum = 10;
//	static constexpr UINT kVertexNum = 2;
//	static UINT drawLineCount;
//	static LineBuffer lineBuffer[kLineMethMaxNum];
//
//	static void Initalize(ID3D12Device* dev);
//	static void PreDraw() { drawLineCount = 0; }
//	static void Draw(const Vector3& v1, const Vector3& v2, const Vector4& color, ConstBuffer<PipelineBase::CommonConstData> camera);
//	static void Draw(const ShapePipeline::Vertex& vertex1, const ShapePipeline::Vertex& vertex2, const ShapePipeline::ConstBufferData& constdata, ConstBuffer<PipelineBase::CommonConstData> camera);
//};
//UINT LineMeth::drawLineCount = 0;
//LineMeth::LineBuffer LineMeth::lineBuffer[LineMeth::kLineMethMaxNum];
//
//void LineMeth::Initalize(ID3D12Device* dev) {
//	for (auto& it : lineBuffer) {
//		it.vertBuff.Create(dev, kVertexNum);
//		it.worldMatBuff.Create(dev);
//		it.vertBuff.Map();
//		it.worldMatBuff.Map();
//	}
//}
//void LineMeth::Draw(const Vector3& v1, const Vector3& v2, const Vector4& color, ConstBuffer<PipelineBase::CommonConstData> camera) {
//	Draw({ v1,color }, { v2,color }, { Matrix44::Identity }, camera);
//}
//
//void LineMeth::Draw(const ShapePipeline::Vertex& vertex1, const ShapePipeline::Vertex& vertex2, const ShapePipeline::ConstBufferData& constdata, ConstBuffer<PipelineBase::CommonConstData> camera) {
//	assert(drawLineCount < kLineMethMaxNum);
//
//	ID3D12GraphicsCommandList* cmdlist = DirectXCommon::GetInstance()->GetInstance()->GetCommandList();
//
//	ShapePipeline::Vertex vertices[kVertexNum];
//
//	vertices[0] = vertex1;
//	vertices[1] = vertex2;
//	std::copy(std::begin(vertices), std::end(vertices), lineBuffer[drawLineCount].vertBuff.GetMapPtr());
//
//	lineBuffer[drawLineCount].worldMatBuff.MapPtr()->worldMat = constdata.worldMat;
//
//	ShapePipeline::GetInstance()->SetPipelineState(cmdlist, kBlendModeAlpha);
//	cmdlist->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
//	lineBuffer[drawLineCount].vertBuff.IASet(cmdlist);
//	camera.SetGraphicsRootConstantBufferView(cmdlist, 0);
//	lineBuffer[drawLineCount].worldMatBuff.SetGraphicsRootConstantBufferView(cmdlist, 1);
//	cmdlist->DrawInstanced(kVertexNum, 1, 0, 0);
//
//	drawLineCount++;
//}


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

	Sprite::StaticInitalize(
		directXCommon, textureManager, 
		winApp->GetWindowWidth(), winApp->GetWindowHeight());

	//PipelineBase* spritePipeline = SpritePipeline::GetInstance();
	//spritePipeline->Create(directXCommon,D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	//PipelineBase* shapePipeline = ShapePipeline::GetInstance();
	//shapePipeline->Create(directXCommon,D3D12_PRIMITIVE_TOPOLOGY_TYPE_LINE);

	UINT testImg = 0;
	UINT aaa;
	testImg = textureManager->LoadTexture(L"resources/images/test1.png");
	aaa = textureManager->LoadTexture(L"resources/images/test.png");

	float theta = 0.0f;

	Sprite test(testImg, { 0,0 }, { 100,100 });
	test.SetTextureRect({ 0.0f,0.0f }, { 256.0f,256.0f });
	//test.SetColor({ 1.0f,0.0f,0.0f,1.0f });
	test.SetPosition({ 640,360 });
	test.SetAnchorPoint({ 50.0f,50.0f });
	
	Camera2DView camera(winApp->GetWindowWidth(), winApp->GetWindowHeight(), false);

	camera.SetAnchorPoint({ 640.0f, 360.0f});
	//camera.SetScale({ 1.0f,1.0f });
	////camera.SetRoatation(Math::ToRadians(5.0f));
    //camera.SetScroll({ 0.0f,0.0f });

	//LineMeth::Initalize(directXCommon->GetDevice());


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

#pragma region 立方体モデル

	//SpritePipeline::Vertex vertices[] =
	//{	// 前
	//	{ {	-5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
	//	{ { -5.0f,  5.0f,  -5.0f }, { 0.0f, 0.0f } },	// 左上
	//	{ {  5.0f, -5.0f,  -5.0f }, { 1.0f, 1.0f } },	// 右下
	//	{ {  5.0f,  5.0f,  -5.0f }, { 1.0f, 0.0f } },	// 右上
	//	// 後
	//	{ {	-5.0f, -5.0f,   5.0f }, { 0.0f, 1.0f } },	// 左下
	//	{ { -5.0f,  5.0f,   5.0f }, { 0.0f, 0.0f } },	// 左上
	//	{ {  5.0f, -5.0f,   5.0f }, { 1.0f, 1.0f } },	// 右下
	//	{ {  5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
	//	// 左
	//	{ {	-5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
	//	{ { -5.0f, -5.0f,   5.0f }, { 0.0f, 0.0f } },	// 左上
	//	{ { -5.0f,  5.0f,  -5.0f }, { 1.0f, 1.0f } },	// 右下
	//	{ { -5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
	//	// 右
	//	{ {	 5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
	//	{ {  5.0f, -5.0f,   5.0f }, { 0.0f, 0.0f } },	// 左上
	//	{ {  5.0f,  5.0f,  -5.0f }, { 1.0f, 1.0f } },	// 右下
	//	{ {  5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
	//	// 上
	//	{ {	-5.0f,  5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
	//	{ {  5.0f,  5.0f,  -5.0f }, { 0.0f, 0.0f } },	// 左上
	//	{ { -5.0f,  5.0f,   5.0f }, { 1.0f, 1.0f } },	// 右下
	//	{ {  5.0f,  5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
	//	// 下
	//	{ {	-5.0f, -5.0f,  -5.0f }, { 0.0f, 1.0f } },	// 左下
	//	{ {  5.0f, -5.0f,  -5.0f }, { 0.0f, 0.0f } },	// 左上
	//	{ { -5.0f, -5.0f,   5.0f }, { 1.0f, 1.0f } },	// 右下
	//	{ {  5.0f, -5.0f,   5.0f }, { 1.0f, 0.0f } },	// 右上
	//};

	//uint16_t indices[] =
	//{	0, 1, 2,
	//	2, 1, 3,

	//	5, 4, 6,
	//	5, 6, 7,

	//	8, 9,10,
	//   10, 9,11,

	//   13,12,14,
	//   13,14,15,

	//   17,16,18,
	//   17,18,19,

	//   21,22,20,
	//   22,21,23
	//};


	//VertexBuffer<SpritePipeline::Vertex> vertBuffer;
	//vertBuffer.Create(directXCommon->GetDevice(), _countof(vertices));
	//vertBuffer.Map();
	//std::copy(std::begin(vertices), std::end(vertices), vertBuffer.GetMapPtr());

	//IndexBuffer indexBuff;
	//indexBuff.Create(directXCommon->GetDevice(), _countof(indices));
	//indexBuff.Map();
	//std::copy(std::begin(indices), std::end(indices), indexBuff.GetMapPtr());
	//indexBuff.Unmap();

#pragma endregion

#pragma region カメラ行列

	// プロジェクション行列
	Matrix44 projectionMat;
	float fovAngleY = Math::ToRadians(45.0f);	// 上下画角
	float aspectRatio = (float)winApp->GetWindowWidth() / (float)winApp->GetWindowHeight();  // アスペクト比
	float nearZ = 0.1f;			// 前端
	float farZ = 1000.0f;		// 奥端

	// ビュー行列
	Matrix44 viewMat;
	Vector3 eye(0,100,100);	// 視点座標
	Vector3 target(0, 0, 0);	// 注視点座標
	Vector3 up(0, 1, 0);		// 上方向ベクトル

	// 透視投影行列の計算
	projectionMat = Matrix44::CreateProjection(fovAngleY, aspectRatio, nearZ, farZ);
	// ビュー変換行列
	viewMat = Matrix44::CreateView(eye, target, up);

	Matrix44 ortMat = Matrix44::CreateOrthoProjection((float)winApp->GetWindowWidth(), (float)winApp->GetWindowHeight());

	/*ConstBuffer<PipelineBase::CommonConstData> commonConstData;
	commonConstData.Create(directXCommon->GetDevice());
	commonConstData.Map();*/

#pragma endregion

	float cameraAngle = 0.0f;

#pragma region 立方体実態

	// 立方体の実態
	const UINT kSpriteCount = 2;

	TransForm spriteTrans[kSpriteCount];
	spriteTrans[0].scale = Vector3(5, 5, 5);
	spriteTrans[1].scale = Vector3(5, 10, 5);
	
	spriteTrans[0].position = Vector3(100, 200, 200);
	spriteTrans[1].position = Vector3(-100, 100, 20);
	spriteTrans[1].angle = Vector3(0, 0, 0);

	/*ConstBuffer<SpritePipeline::ConstBufferData> spriteConstBuffer[kSpriteCount];
	for (auto& it : spriteConstBuffer) {
		it.Create(directXCommon->GetDevice());
		it.Map();
	}*/

	UINT handle[2] = { testImg, aaa };

#pragma endregion

#pragma region 軸

	float axisLength = 1000.0f;
	

#pragma endregion
	int n = 0;

	float theta1 = 0.0f;
	float theta2 = 0.0f;

	while (!winApp->WindowQUit()) {

		input->Update();
		directXCommon->PreDraw();
	
		//LineMeth::PreDraw();

		// AD入力でカメラが原点の周りを回る
		if (input->IsKeyPressed(DIK_D)) {
			eye.x += 5;
		}
		if (input->IsKeyPressed(DIK_A)) {
			eye.x += -5;
		}

		if (input->IsKeyPressed(DIK_W)) {
			eye.y += 5;
		}
		if (input->IsKeyPressed(DIK_S)) {
			eye.y += -5;
		}

		if (input->IsKeyPressed(DIK_E)) {
			eye.z += 5;
		}
		if (input->IsKeyPressed(DIK_Q)) {
			eye.z += -5;
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
		

		

		if (input->IsKeyPressed(DIK_1)) {
			spriteTrans[1].angle.x += Math::ToRadians(1.0f);
		}
		if (input->IsKeyPressed(DIK_2)) {
			spriteTrans[1].angle.y += Math::ToRadians(1.0f);
		}
		if (input->IsKeyPressed(DIK_3)) {
			spriteTrans[1].angle.z += Math::ToRadians(1.0f);
		}
		if (input->IsKeyTrigger(DIK_T)) {
			spriteTrans[1].angle *= 0.0f;
		}

		auto theta = camera.GetRotation();
		theta += Math::ToRadians(1.0f);
		
		auto pos = camera.GetScroll();

		if (input->IsKeyPressed(DIK_D)) {
			pos.x += 5;
		}
		if (input->IsKeyPressed(DIK_A)) {
			pos.x += -5;
		}
		
		camera.SetRoatation(theta);
		camera.SetScroll(pos);

		//camera.SetRoatation(theta);
		///
		/// 描画処理はここから
		///

		camera.SetUpMatrix();


		// 透視投影行列の計算
		projectionMat = Matrix44::CreateProjection(fovAngleY, aspectRatio, nearZ, farZ);
		// ビュー変換行列
		viewMat = Matrix44::CreateView(eye, target, up);
		// カメラバッファに送る
		//commonConstData.MapPtr()->cameraMat = viewMat * projectionMat;
		//theta += Math::ToRadians(1.0f);
		//test.SetRotation(theta);
		Sprite::Draw(test, &camera, kBlendModeNormal);
		

		//Matrix44 lookatmat = Matrix44::CreateLookAt(eye - spriteTrans[1].position, Vector3::UnitY);
		//Matrix44 lookatmat2 = Matrix44::CreateLookAt( spriteTrans[1].position - spriteTrans[0].position, Vector3::UnitY);
		//spriteTrans[0].worldMat = Matrix44::CreateScaling(spriteTrans[0].scale) * lookatmat2 * Matrix44::CreateTranslation(spriteTrans[0].position);
		//spriteTrans[1].worldMat = Matrix44::CreateScaling(spriteTrans[1].scale) * lookatmat * Matrix44::CreateTranslation(spriteTrans[1].position);

		//// 軸描画
		//LineMeth::Draw(Vector3::UnitX* axisLength, Vector3::UnitX * -axisLength, Color::Red, commonConstData);
		//LineMeth::Draw(Vector3::UnitY* axisLength, Vector3::UnitY * -axisLength, Color::Green, commonConstData);
		//LineMeth::Draw(Vector3::UnitZ* axisLength, Vector3::UnitZ * -axisLength, Color::Blue, commonConstData);

		//theta1 += 0.03f;
		//Matrix44 rm1 = Matrix44::CreateRotation({ Normalize(eye - spriteTrans[0].position), theta1 });
		//theta2 += 0.05f;
		//Matrix44 rm2 = Matrix44::CreateRotation({ Normalize(spriteTrans[0].position - spriteTrans[1].position), theta2 });
		//spriteTrans[0].worldMat = Matrix44::CreateScaling(spriteTrans[0].scale) * rm1 * Matrix44::CreateTranslation(spriteTrans[0].position);
		//spriteTrans[1].worldMat = Matrix44::CreateScaling(spriteTrans[1].scale) * rm2 * Matrix44::CreateTranslation(spriteTrans[1].position);

		//LineMeth::Draw(Normalize(eye - spriteTrans[0].position) * 1000 + spriteTrans[0].position, spriteTrans[0].position, Color::White, commonConstData);
		//LineMeth::Draw(spriteTrans[0].position, spriteTrans[1].position, Color::White, commonConstData);
	

		//// スプライト描画
		//for (int i = 0; i < kSpriteCount; i++) {
		//
		//	//spriteTrans[i].CalcTransFormMatrix();
		//	spriteConstBuffer[i].MapPtr()->worldMat = spriteTrans[i].worldMat;
		//	spriteConstBuffer[i].MapPtr()->color = Vector4{ 1,1,1,1 };
		//
		//	spritePipeline->SetPipelineState(directXCommon->GetCommandList(), kBlendModeAlpha);
		//	directXCommon->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		//	vertBuffer.IASet(directXCommon->GetCommandList());
		//	indexBuff.IASet(directXCommon->GetCommandList());
		//	commonConstData.SetGraphicsRootConstantBufferView(directXCommon->GetCommandList(), 0);
		//	spriteConstBuffer[i].SetGraphicsRootConstantBufferView(directXCommon->GetCommandList(), 1);
		//	textureManager->SetGraphicsRootDescriptorTable(directXCommon->GetCommandList(), 2, handle[i]);
		//
		//
		//	directXCommon->GetCommandList()->DrawIndexedInstanced(
		//		indexBuff.GetIndexCount(), 1, 0, 0, 0);
		//}


		


		directXCommon->PostDraw();
	}


	winApp->Finalize();

	return 0;
}



