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
	Vector3 m_scale = { 1.0f,1.0f,1.0f };
	Vector3 m_rotation = { 0.0f,0.0f,0.0f };
	Vector3 m_position = { 0.0f,0.0f,0.0f };
	Matrix44 m_worldMatrix;

	TransForm* m_parent = nullptr;

	void UpdateMatrix() {
		Matrix44 scaMat, rotMat, traMat;

		scaMat = Matrix44::CreateScaling(m_scale);

		rotMat *= Matrix44::CreateRotationZ(m_rotation.z);
		rotMat *= Matrix44::CreateRotationX(m_rotation.x);
		rotMat *= Matrix44::CreateRotationY(m_rotation.y);

		traMat *= Matrix44::CreateTranslation(m_position);

		m_worldMatrix = Matrix44::Identity;
		m_worldMatrix *= scaMat;
		m_worldMatrix *= rotMat;
		m_worldMatrix *= traMat;

		if (m_parent != nullptr) {
			m_worldMatrix *= m_parent->m_worldMatrix;
		}
	}
};

class FpsCameraObject 
{
private:
	Camera3D m_camera;
	Vector3 m_defaultForward;	// 基準となる前方方向
	Vector3 m_ray;	// 視線
	Vector2 m_rayAngle;	// 視線の角度
	Vector2 m_rayAngularSpeed;	// 振り向きの速度上下左右
	Vector2 m_rayAngleVerticalLimit; // 視線の上下方向の上限
	float m_moveSpeed;
	Twins<int> m_lookInput; // 視線の移動の入力 x 左右振り向き : y 上下振り向き
	Triplet<int> m_moveInput;	// 視線に対して x 左右移動 : y 上下移動 : z 前後移動

public:
	Camera3D* GetCamera() { return &m_camera; }

	FpsCameraObject() 
	{
		m_camera.SetPosition({ 0.0f, 0.0f, -10.0f });;
		m_defaultForward = Vector3::Normalize({ 0.0f,0.0f,1.0f });
		m_ray = m_defaultForward;
		m_camera.SetRay(m_ray);
		m_rayAngle = { 0.0f,0.0f };
		m_rayAngularSpeed = { Math::ToRadians(1.5f),Math::ToRadians(1.5f) };
		m_rayAngleVerticalLimit = { Math::ToRadians(-80.0f),Math::ToRadians(89.0f) };
		m_moveSpeed = 0.3f;
		m_lookInput.Set(0, 0);
		m_moveInput.Set(0, 0, 0);
	}

	void Update(Input* input) 
	{
		Input(input);

		m_rayAngle.x += m_rayAngularSpeed.x * m_lookInput.x;
		m_rayAngle.y += m_rayAngularSpeed.y * m_lookInput.y;
		m_rayAngle.y = Math::Clamp<float>(m_rayAngle.y, m_rayAngleVerticalLimit.x, m_rayAngleVerticalLimit.y);

		m_ray = m_defaultForward.RotationX(m_rayAngle.y).RotationY(m_rayAngle.x);
		
		Vector3 pos = m_camera.GetPosition();
		// y軸に平行な前方向
		Vector3 forward = Vector3::Normalize({ m_ray.x,0,m_ray.z });
		Vector3 right = -Normalize(forward.Cross(Vector3::UnitY));

		pos += right * m_moveSpeed * m_moveInput.x;
		pos += Vector3::UnitY * m_moveSpeed * m_moveInput.y;
		pos += forward * m_moveSpeed * m_moveInput.z;

		m_camera.SetPosition(pos);
		m_camera.SetRay(m_ray);
		m_camera.UpdateMatrix();
	}

private:
	void Input(Input* input)
	{
		// 入力を初期に戻す
		m_lookInput.Set(0, 0);
		m_moveInput.Set(0, 0, 0);

		// 視線移動の入力
		if (input->IsKeyPressed(DIK_RIGHT)) {
			m_lookInput.x += 1;
		}
		if (input->IsKeyPressed(DIK_LEFT)) {
			m_lookInput.x += -1;
		}

		if (input->IsKeyPressed(DIK_UP)) {
			m_lookInput.y += -1;
		}
		if (input->IsKeyPressed(DIK_DOWN)) {
			m_lookInput.y += 1;
		}


		if (input->IsKeyPressed(DIK_W)) {
			m_moveInput.z += 1;
		}
		if (input->IsKeyPressed(DIK_S)) {
			m_moveInput.z += -1;
		}
		if (input->IsKeyPressed(DIK_D)) {
			m_moveInput.x += 1;
		}
		if (input->IsKeyPressed(DIK_A)) {
			m_moveInput.x += -1;
		}
		if (input->IsKeyPressed(DIK_SPACE)) {
			m_moveInput.y += 1;
		}
		if (input->IsKeyPressed(DIK_LSHIFT)) {
			m_moveInput.y += -1;
		}
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
	textureManager->LoadTexture("resources/images/white.png");

	Input* input = Input::GetInstance();
	input->Initialize(winApp);

	Sprite::StaticInitalize(
		directXCommon, textureManager, 
		winApp->GetWindowWidth(), winApp->GetWindowHeight());

	Model::StaticInitalize(directXCommon, textureManager);

	UINT testImg = textureManager->LoadTexture("resources/images/test1.png");
	UINT test1Img = textureManager->LoadTexture("resources/images/test.png");
	UINT carrotImg = textureManager->LoadTexture("resources/model/carrot/Carrot.png");


	// カメラセット

	/*Camera3D camera;

	camera.SetPosition({0.0f,0.0f,-10.0f});
	
	camera.SetTarget({ 0.0f,0.0f,0.0f});
	Vector3 cameraDefultForward = {0.0f,0.0f,10.0f};
	Vector2 cameraEyeVecRoatation = { 0.0f,0.0f };
	Vector2 cameraEyeVecAngularSpeed = { Math::ToRadians(1.0f) ,Math::ToRadians(1.0f) };
	float cameraForwardSpeed = 1.0f;
	constexpr float eyeYMin = Math::ToRadians(-70.0f);
	constexpr float eyeYMax = Math::ToRadians(89.9f);
	Triplet<int> cameraMoveInput;*/

	FpsCameraObject cameraObj;

	auto cube = Model::CreateFromObj("sphere/ICOSphere.obj");
	auto uvsp = Model::CreateFromObj("sphere/UVSphere.obj");
	auto carrot = Model::CreateFromObj("carrot/Carrot.obj");
	//auto sphere = Model::CreateFromObj("sphere/Sphere.obj");
	
	const size_t kObjNum = 3;
	std::vector<Object3D> obj(kObjNum);

	for (int i = 0; i < kObjNum; i++) {
		obj[i].SetCamera(cameraObj.GetCamera());
		obj[i].SetPosition({ 5.0f * i,0.0f,0.0f });
		//obj[i].SetRotation({ Math::ToRadians(90),0.0f, 0.0f});
		obj[i].SetModel(cube.get());
	}
	obj[1].SetModel(carrot.get());
	obj[2].SetModel(uvsp.get());

	//obj[1].SetModel(carrot.get());
	//obj[2].SetModel(sphere.get());


	directXCommon->SetClearColor(Color::ToVector4(0x000000FF));

	Sprite _00(testImg, { 0,0 }, { 64,64 });
	_00.SetTextureRect({ 0.0f,0.0f }, { 256.0f,256.0f });

	Camera2DView camera2D(winApp->GetWindowWidth(), winApp->GetWindowHeight());

	camera2D.SetAnchorPoint({ 640.0f, 360.0f});

	Quaternion q1(Normalize(Vector3{10,10,10}), -Math::ToRadians(45));
	Quaternion q2(Normalize(Vector3{ -10,10,10 }), -Math::ToRadians(250));
	float t = 0;
	float tspe = 1.0f / 180.0f;

	for (auto& it : obj) {
		it.UpdateMatrix();
	}
	
	float theta = 0;
	float cth = 0.0f;
	while (!winApp->WindowQUit()) {

		directXCommon->PreDraw();
		input->Update();

		
		cameraObj.Update(input);
		
	

		theta += Math::ToRadians(1.0f);

		for (auto& it : obj) {
			auto rota = it.GetRotation();
			it.SetRotation({ rota.x,theta,rota.z });
		}

		t += tspe;

		if (t < 0 || 1 < t) {
			tspe = -tspe;
		}

		Matrix44 scaleMat = Matrix44::CreateScaling(obj[1].GetScale());
		Matrix44 transMat = Matrix44::CreateTranslation(obj[1].GetPosition());
		Matrix44 rotMat = Matrix44::CreateRotationFromQuaternion(Quaternion::Slerp(t, q1, q2));
	


		///
		/// 描画処理はここから
		///
		
		//for (auto& it : obj) {
		//	it.UpdateMatrix();
		//}
		//obj[1].SetWorldMatrix(scaleMat * rotMat * transMat);

		camera2D.SetUpMatrix();

		//Sprite::Draw(_00, &camera2D, kBlendModeNormal);
		
		obj[0].Draw();
		obj[1].Draw();
		obj[2].Draw();


		directXCommon->PostDraw();
	}


	winApp->Finalize();

	return 0;
}



