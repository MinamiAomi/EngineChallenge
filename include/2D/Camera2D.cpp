#include "Camera2D.h"

Camera2D::Camera2D() {}

Camera2D::Camera2D(unsigned int width, unsigned int height) :
	windowWidth(width), windowHeight(height) {}

void Camera2D::SetUpMatrix() {
	projectionMat = Matrix44::CreateOrthoProjection(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
	transformMat = projectionMat;
}

/////////////////////////

Camera2DView::Camera2DView() {}
Camera2DView::Camera2DView(unsigned int width, unsigned int height) :
	Camera2D(width, height) {}

void Camera2DView::SetUpMatrix() {
	// 射影行列をセット
	Camera2D::SetUpMatrix();
	// ビュー行列をセット
	viewMat = Matrix44::Create2DView(anchorPoint, scroll, scale, rotation);
	// ビュー * 射影
	SetTransformMatrix(viewMat * GetTransformMatrix());
}