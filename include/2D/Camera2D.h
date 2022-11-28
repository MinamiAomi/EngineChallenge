#pragma once
#include "MathUtility.h"

// 射影行列行列のみ
class Camera2D
{


private:
	
	// スクリーン座標系
	unsigned int windowWidth = 1280;
	unsigned int windowHeight = 720;

	Matrix44 projectionMat;
	Matrix44 transformMat;

public:
	Camera2D();
	Camera2D(unsigned int width, unsigned int height);
	virtual ~Camera2D() {}

	virtual void SetUpMatrix();
	
	void SetWindowWidth(unsigned int width) { windowWidth = width; }
	unsigned int GetWindowWidth() const { return windowWidth; }
	void SetWindowHeight(unsigned int height) { windowHeight = height; }
	unsigned int GetWindowHeight() const { return windowHeight; }
	void SetProjectionMatrix(const Matrix44& mat) { projectionMat = mat; }
	const Matrix44& GetProjectionMatrix() const { return projectionMat; }
	void SetTransformMatrix(const Matrix44& mat) {	transformMat = mat; }
	const Matrix44& GetTransformMatrix() const { return transformMat; }

};

// ビュー行列あり
class Camera2DView : 
	public Camera2D 
{
private:

	Vector2 scroll = { 0.0f,0.0f };
	Vector2 anchorPoint = { 0.0f,0.0f };
	Vector2 scale = { 1.0f,1.0f };
	float rotation = 0.0f;

	Matrix44 viewMat;

public:
	Camera2DView();
	Camera2DView(unsigned int width, unsigned int height);

	void SetUpMatrix();

	void SetScroll(const Vector2& scr) { scroll = scr; }
	const Vector2& GetScroll() const { return scroll; }
	void SetAnchorPoint(const Vector2& point) { anchorPoint = point; }
	const Vector2& GetAnchorPoint() const { return anchorPoint; }
	void SetScale(const Vector2& sca) { scale = sca; }
	const Vector2& GetScale() const { return scale; }
	void SetRoatation(float theta) { rotation = theta; }
	float GetRotation() const { return rotation; }
	void SetViewMatrix(const Matrix44 mat) { viewMat = mat; }
	const Matrix44& GetViewMatrix() const { return viewMat; }
};