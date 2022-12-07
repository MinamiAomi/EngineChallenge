#include "Object3D.h"
#include "DirectXCommon.h"
#include "Camera3D.h"

Object3D::Object3D() 
{
	auto dixcom = DirectXCommon::GetInstance();
	assert(dixcom->IsInit());
	m_constBuffer.Create(dixcom->GetDevice());
	m_constBuffer.Map();
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

void Object3D::Draw()
{
	auto dixcom = DirectXCommon::GetInstance();
	assert(dixcom->IsInit());
	auto cmdList = dixcom->GetCommandList();

	m_model->Draw(cmdList, this);
}
void Object3D::Draw(Model* model)
{
	auto dixcom = DirectXCommon::GetInstance();
	assert(dixcom->IsInit());
	auto cmdList = dixcom->GetCommandList();

	model->Draw(cmdList, this);
}

void Object3D::Transfer(ID3D12GraphicsCommandList* cmdList) 
{
	m_constBuffer.MapPtr()->world = m_worldMatrix;
	m_constBuffer.SetGraphicsRootConstantBufferView(cmdList, Model::kWorldTransform);
	m_camera->SetGraphicsCommand(cmdList, Model::kCamera);
}