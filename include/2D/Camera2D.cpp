#include "Camera2D.h"

Camera2D::Camera2D() {}

Camera2D::Camera2D(unsigned int width, unsigned int height, bool isScreen) :
	windowWidth(width), windowHeight(height), isScreen(isScreen) {}

void Camera2D::SetUpMatrix() {
	projectionMat = Matrix44::CreateOrthoProjection(static_cast<float>(windowWidth), static_cast<float>(windowHeight));
	if (isScreen == false) {
		//projectionMat = Matrix44::CreateLeftBottomOrigin(static_cast<float>(windowHeight)) * projectionMat;
	}
	transformMat = projectionMat;
}

/////////////////////////

Camera2DView::Camera2DView() {}
Camera2DView::Camera2DView(unsigned int width, unsigned int height, bool isScreen) :
	Camera2D(width, height, isScreen) {}

void Camera2DView::SetUpMatrix() {
	// �ˉe�s����Z�b�g
	Camera2D::SetUpMatrix();
	// �r���[�s����Z�b�g
	viewMat = Matrix44::Create2DView(anchorPoint, scroll, scale, rotation);
	// �r���[ * �ˉe
	SetTransformMatrix(viewMat * GetTransformMatrix());
}