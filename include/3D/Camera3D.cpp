#include "Camera3D.h"
#include "DirectXCommon.h"

Camera3D::Camera3D()
{
	auto dixcom = DirectXCommon::GetInstance();
	assert(dixcom->IsInit());
	m_constBuffer.Create(dixcom->GetDevice());
	m_constBuffer.Map();
}

void Camera3D::UpdateMatrix()
{
	m_viewMat = Matrix44::CreateView(m_eye, m_target, m_up);
	m_projMat = Matrix44::CreateProjection(m_fovAngleY, m_aspectRatio, m_nearZ, m_farZ);
	m_viewProjMat = m_viewMat * m_projMat;

}

void Camera3D::SetGraphicsCommand(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndexCamera)
{
	m_constBuffer.MapPtr()->view = m_viewMat;
	m_constBuffer.MapPtr()->proj = m_projMat;
	m_constBuffer.MapPtr()->cameraPos = m_eye;
	m_constBuffer.SetGraphicsRootConstantBufferView(cmdList, rootParameterIndexCamera);
}
