#include "Object3D.h"
#include "DirectXCommon.h"
#include "Camera3D.h"

DirectXCommon* Object3D::dixCom = nullptr;

void Object3D::StaticInitalize(DirectXCommon* dixcom) 
{
	assert(dixcom != nullptr);
	dixCom = dixcom;
}

Object3D::Object3D() 
{
	m_constBufferTransform.Create(dixCom->GetDevice());
	m_constBufferTransform.Map();
}
Object3D::~Object3D() 
{
	dixCom = nullptr;
}

void Object3D::UpdateMatrix() 
{
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

void Object3D::Draw(UINT tex)
{
	auto* cmdList = dixCom->GetCommandList();

	m_constBufferTransform.MapPtr()->viewProj = m_camera->GetViewProjMat();
	m_constBufferTransform.MapPtr()->world = m_worldMatrix;
	
	Model::PreDraw(m_type);
	m_constBufferTransform.SetGraphicsRootConstantBufferView(cmdList, Model::kTransform);
	m_model->Draw(m_type,tex);
}
void Object3D::Draw(Model* model, Camera3D* camera, UINT tex) 
{
	auto* cmdList = dixCom->GetCommandList();

	m_constBufferTransform.MapPtr()->viewProj = camera->GetViewProjMat();
	m_constBufferTransform.MapPtr()->world = m_worldMatrix;

	Model::PreDraw(m_type);
	m_constBufferTransform.SetGraphicsRootConstantBufferView(cmdList, Model::kTransform);
	model->Draw(m_type, tex);
}