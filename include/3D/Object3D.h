#pragma once

#include "ConstBuffer.h"
#include "MathUtility.h"
#include "Model.h"

class Object3D
{

public:
	struct ConstBudderDataTransform
	{
		Matrix44 world;
	};

private:
	ConstBuffer<ConstBudderDataTransform> m_constBuffer;
	Vector3 m_scale = { 1.0f,1.0f,1.0f };
	Vector3 m_rotation = { 0.0f,0.0f,0.0f };
	Vector3 m_position = { 0.0f,0.0f,0.0f };
	Matrix44 m_worldMatrix;
	Object3D* m_parent = nullptr;
	class Model* m_model = nullptr;
	class Camera3D* m_camera = nullptr;


public:
	void SetPosition(const Vector3& pos) { m_position = pos; }
	const Vector3& GetPosition() const { return m_position; }
	void SetRotation(const Vector3& rota) { m_rotation = rota; }
	const Vector3& GetRotation() const { return m_rotation; }
	void SetScale(const Vector3& sca) { m_scale = sca; }
	const Vector3& GetScale() const { return m_scale; }
	void SetWorldMatrix(const Matrix44& mat) { m_worldMatrix = mat; }
	const Matrix44& GetWorldMatrix() const { return m_worldMatrix; }
	void SetParent(Object3D& obj) { m_parent = &obj; }
	Object3D* GetParent() const { return m_parent; }
	void SetModel(Model& model) { m_model = &model; }
	void SetModel(Model* model) { m_model = model; }
	void SetCamera(Camera3D& camera) { m_camera = &camera; }
	void SetCamera(Camera3D* camera) { m_camera = camera; }

	Object3D();
	~Object3D() {}
	
	void UpdateMatrix();

	void Draw();
	void Draw(Model* model);
	void Transfer(ID3D12GraphicsCommandList* cmdList);
};