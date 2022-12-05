#include "WinApp.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "Input.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "ConstBuffer.h"
#include "MathUtility.h"

#include "Camera2D.h"
#include "Sprite.h"
#include "Camera3D.h"
#include "Model.h"
#include "Object3D.h"



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



// Windowsアプリのエントリーポイント(main関数)
int MAIN
{
	const int windowWidth = 1280;
	const int windowHeight = 720;



	WinApp* winApp = WinApp::GetInstance();
	winApp->Initialize(windowWidth, windowHeight,"Engine");

	DirectXCommon* directXCommon = DirectXCommon::GetInstance();
	directXCommon->Initialize(winApp);

	TextureManager* textureManager = TextureManager::GetInstance();
	textureManager->Initialize(directXCommon);

	Input* input = Input::GetInstance();
	input->Initialize(winApp);

	Sprite::StaticInitalize(
		directXCommon, textureManager, 
		winApp->GetWindowWidth(), winApp->GetWindowHeight());
	textureManager->LoadTexture("resources/images/white.png");

	Model::StaticInitalize(directXCommon, textureManager);
	Object3D::StaticInitalize(directXCommon);

	UINT testImg = textureManager->LoadTexture("resources/images/test1.png");
	UINT test1Img = textureManager->LoadTexture("resources/images/test.png");
	UINT carrotImg = textureManager->LoadTexture("resources/model/carrot/Carrot.png");

	Camera3D camera;

	camera.SetPosition({0.0f,10.0f,-10.0f});

	auto cube = Model::CreateFromObj("cube/Cube.obj");
	auto carrot = Model::CreateFromObj("carrot/Carrot.obj");
	auto sphere = Model::CreateFromObj("sphere/Sphere.obj");
	
	const size_t kObjNum = 3;
	std::vector<Object3D> obj(kObjNum);

	for (int i = 0; i < kObjNum; i++) {
		obj[i].SetCamera(camera);
		obj[i].SetPosition({ 5.0f * i,0.0f,0.0f });
		obj[i].SetRotation({ 0.0f,0.0f, -Math::ToRadians(45.0f) });
	}

	obj[0].SetModel(cube.get());
	obj[0].SetType(Model::kLineList);
	obj[1].SetModel(carrot.get());
	obj[1].SetType(Model::kLineStrip);
	obj[2].SetModel(sphere.get());


	directXCommon->SetClearColor(Color::ToVector4(0x000000FF));

	Sprite _00(testImg, { 0,0 }, { 64,64 });
	_00.SetTextureRect({ 0.0f,0.0f }, { 256.0f,256.0f });

	Camera2DView camera2D(winApp->GetWindowWidth(), winApp->GetWindowHeight());

	camera2D.SetAnchorPoint({ 640.0f, 360.0f});

	
	float theta = 0;

	while (!winApp->WindowQUit()) {

		directXCommon->PreDraw();
		input->Update();

		auto cameraPos = camera.GetPosition();
		Vector3 vel;

		if (input->IsKeyPressed(DIK_E)) {
			vel.x += 1;
		}
		if (input->IsKeyPressed(DIK_Q)) {
			vel.x -= 1;
		}
		camera.SetPosition(cameraPos + vel);
		if (input->IsKeyPressed(DIK_LSHIFT)) {
			camera.SetTarget(camera.GetTarget() + vel);
		}



		theta += Math::ToRadians(1.0f);

		for (auto& it : obj) {
			auto rota = it.GetRotation();
			it.SetRotation({ rota.x,theta,rota.z });
		}



		///
		/// 描画処理はここから
		///
		for (auto& it : obj) {
			it.UpdateMatrix();
		}

		camera.UpdateMatrix();
		camera2D.SetUpMatrix();

		//Sprite::Draw(_00, &camera2D, kBlendModeNormal);
		
		obj[0].Draw();
		obj[1].Draw(carrotImg);
		obj[2].Draw();


		directXCommon->PostDraw();
	}


	winApp->Finalize();

	return 0;
}



