#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include <math.h>

class Matrix44
{

public:
	static const Matrix44 Identity;

private:
	float m[4][4];

public:
	Matrix44(
		float _00 = 1.0f, float _01 = 0.0f, float _02 = 0.0f, float _03 = 0.0f,
		float _10 = 0.0f, float _11 = 1.0f, float _12 = 0.0f, float _13 = 0.0f,
		float _20 = 0.0f, float _21 = 0.0f, float _22 = 1.0f, float _23 = 0.0f,
		float _30 = 0.0f, float _31 = 0.0f, float _32 = 0.0f, float _33 = 1.0f) {
		m[0][0] = _00, m[0][1] = _01, m[0][2] = _02, m[0][3] = _03;
		m[1][0] = _10, m[1][1] = _11, m[1][2] = _12, m[1][3] = _13;
		m[2][0] = _20, m[2][1] = _21, m[2][2] = _22, m[2][3] = _23;
		m[3][0] = _30, m[3][1] = _31, m[3][2] = _32, m[3][3] = _33;
	}

	inline const float* operator[](size_t i) const {
		return m[i];
	}

	friend inline Matrix44 operator*(const Matrix44& a, const Matrix44& b) {
		return {
			a.m[0][0] * b.m[0][0] + a.m[0][1] * b.m[1][0] + a.m[0][2] * b.m[2][0] + a.m[0][3] * b.m[3][0],
			a.m[0][0] * b.m[0][1] + a.m[0][1] * b.m[1][1] + a.m[0][2] * b.m[2][1] + a.m[0][3] * b.m[3][1],
			a.m[0][0] * b.m[0][2] + a.m[0][1] * b.m[1][2] + a.m[0][2] * b.m[2][2] + a.m[0][3] * b.m[3][2],
			a.m[0][0] * b.m[0][3] + a.m[0][1] * b.m[1][3] + a.m[0][2] * b.m[2][3] + a.m[0][3] * b.m[3][3],

			a.m[1][0] * b.m[0][0] + a.m[1][1] * b.m[1][0] + a.m[1][2] * b.m[2][0] + a.m[1][3] * b.m[3][0],
			a.m[1][0] * b.m[0][1] + a.m[1][1] * b.m[1][1] + a.m[1][2] * b.m[2][1] + a.m[1][3] * b.m[3][1],
			a.m[1][0] * b.m[0][2] + a.m[1][1] * b.m[1][2] + a.m[1][2] * b.m[2][2] + a.m[1][3] * b.m[3][2],
			a.m[1][0] * b.m[0][3] + a.m[1][1] * b.m[1][3] + a.m[1][2] * b.m[2][3] + a.m[1][3] * b.m[3][3],

			a.m[2][0] * b.m[0][0] + a.m[2][1] * b.m[1][0] + a.m[2][2] * b.m[2][0] + a.m[2][3] * b.m[3][0],
			a.m[2][0] * b.m[0][1] + a.m[2][1] * b.m[1][1] + a.m[2][2] * b.m[2][1] + a.m[2][3] * b.m[3][1],
			a.m[2][0] * b.m[0][2] + a.m[2][1] * b.m[1][2] + a.m[2][2] * b.m[2][2] + a.m[2][3] * b.m[3][2],
			a.m[2][0] * b.m[0][3] + a.m[2][1] * b.m[1][3] + a.m[2][2] * b.m[2][3] + a.m[2][3] * b.m[3][3],

			a.m[3][0] * b.m[0][0] + a.m[3][1] * b.m[1][0] + a.m[3][2] * b.m[2][0] + a.m[3][3] * b.m[3][0],
			a.m[3][0] * b.m[0][1] + a.m[3][1] * b.m[1][1] + a.m[3][2] * b.m[2][1] + a.m[3][3] * b.m[3][1],
			a.m[3][0] * b.m[0][2] + a.m[3][1] * b.m[1][2] + a.m[3][2] * b.m[2][2] + a.m[3][3] * b.m[3][2],
			a.m[3][0] * b.m[0][3] + a.m[3][1] * b.m[1][3] + a.m[3][2] * b.m[2][3] + a.m[3][3] * b.m[3][3] };
	}

	inline Matrix44& operator*=(const Matrix44& mat) {
		*this = *this * mat;
		return *this;
	}

	friend inline Vector3 operator*(const Vector3& vec, const Matrix44& mat) {
		return{
			vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0] + mat.m[3][0],
			vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1] + mat.m[3][1],
			vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2] + mat.m[3][2] };
	}

	static inline Matrix44 CreateIdentity() {
		return{ 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };
	}

	static inline Matrix44 CreateScaling(float scaleX, float scaleY, float scaleZ) {
		return{ scaleX,		0.0f,		0.0f,		0.0f,
				0.0f,		scaleY,		0.0f,		0.0f,
				0.0f,		0.0f,		scaleZ,		0.0f,
				0.0f,		0.0f,		0.0f,		1.0f };
	}
	static inline Matrix44 CreateScaling(float scale) {
		return CreateScaling(scale, scale, scale);
	};
	static inline Matrix44 CreateScaling(const Vector3& scale) {
		return CreateScaling(scale.x, scale.y, scale.z);
	}

	static inline Matrix44 CreateRotationX(float theta) {
		float c = cosf(theta);
		float s = sinf(theta);
		return{ 1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		c,			s,			0.0f,
				0.0f,		-s,			c,			0.0f,
				0.0f,		0.0f,		0,			1.0f };
	}
	static inline Matrix44 CreateRotationY(float theta) {
		float c = cosf(theta);
		float s = sinf(theta);
		return{ c,			0.0f,		-s,			0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				s,			0.0f,		c,			0.0f,
				0.0f,		0.0f,		0,			1.0f };
	}
	static inline Matrix44 CreateRotationZ(float theta) {
		float c = cosf(theta);
		float s = sinf(theta);
		return{ c,			s,			0.0f,		0.0f,
				-s,			c,			0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				0.0f,		0.0f,		0,			1.0f };
	}
	static inline Matrix44 CreateRotationXYZ(float thetaX, float thetaY, float thetaZ) {
		return CreateRotationX(thetaX) * CreateRotationY(thetaY) * CreateRotationZ(thetaZ);
	}
	static inline Matrix44 CreateRotationXYZ(const Vector3& theta) {
		return CreateRotationXYZ(theta.x, theta.y, theta.z);
	}

	static inline Matrix44 CreateRotationZXY(float thetaX, float thetaY, float thetaZ) {
		return CreateRotationZ(thetaZ) * CreateRotationX(thetaX) * CreateRotationY(thetaY);
	}
	static inline Matrix44 CreateRotationZXY(const Vector3& theta) {
		return CreateRotationXYZ(theta.x, theta.y, theta.z);
	}

	//static inline Matrix44 CreateRotationRollPitchYaw((float thetaX, float thetaY, float thetaZ) {

	//}

	static Matrix44 CreateRotation(const class Quaternion&);
	static Matrix44 CreateRotationFromQuaternion(const class Quaternion&);

	static inline Matrix44 CreateTranslation(float x, float y, float z) {
		return{ 1.0f,	0.0f,	0.0f,	0.0f,
				0.0f,	1.0f,	0.0f,	0.0f,
				0.0f,	0.0f,	1.0f,	0.0f,
				x,		y,		z,		1.0f };
	}
	static inline Matrix44 CreateTranslation(const Vector3& trans) {
		return CreateTranslation(trans.x, trans.y, trans.z);
	}

	/// 向かせる行列 （向きベクトルと上ベクトルで作成）
	static inline Matrix44 CreateLookAt(const Vector3& lookat, const Vector3& up) {
		Vector3 z = Normalize(lookat);
		Vector3 x = Normalize(Cross(up, z));
		Vector3 y = Cross(z, x);
		return{
			x.x,	x.y,	x.z,	0.0f,
			y.x,	y.y,	y.z,	0.0f,
			z.x,	z.y,	z.z,	0.0f,
			0.0f,	0.0f,	0.0f,	1.0f };
	}


	static inline Matrix44 CreateView(const Vector3& eye, const Vector3& target, const Vector3& up) {
		Vector3 z = Normalize(target - eye);
		Vector3 x = Normalize(Cross(up, z));
		Vector3 y = Cross(z, x);
		Vector3 w = { -Dot(x,eye),-Dot(y,eye),-Dot(z,eye) };
		return{
			x.x,y.x,z.x,0.0f,
			x.y,y.y,z.y,0.0f,
			x.z,y.z,z.z,0.0f,
			w.x,w.y,w.z,1.0f };
	}

	static inline Matrix44 CreateProjection(float fovY, float aspect, float nearZ, float farZ) {
		float s = 1.0f / tanf(fovY / 2.0f);
		float a = farZ / (farZ - nearZ);
		return {
			 s / aspect,	0.0f,		0.0f,			0.0f ,
			 0.0f,			s,			0.0f,			0.0f ,
			 0.0f,			0.0f,		a,				1.0f ,
			 0.0f,			0.0f,		a * -nearZ,		0.0f };
	}

	static inline Matrix44 Create2DView(const Vector2& anchorPoint, const Vector2& scroll, const Vector2& scale, float theta) {
		Matrix44 result = CreateTranslation({ -anchorPoint, 0.0f });
		result *= CreateScaling({ scale, 0.0f });
		result *= CreateRotationZ(-theta);
		result *= CreateTranslation({ -scroll,0.0f });
		result *= CreateTranslation({ anchorPoint, 0.0f });
		return result;
	}

	static inline Matrix44 CreateOrthoProjection(float width, float height, float nearZ = 0.0f, float farZ = 1.0f) {
		return {
			 2.0f / width,	0.0f,				0.0f,					0.0f,
			 0.0f,			-2.0f / height,		0.0f,					0.0f,
			 0.0f,			0.0f,				1.0f / (farZ - nearZ),	0.0f,
			 -1.0f,			1.0f,				nearZ / (nearZ - farZ),	1.0f };
	}
};
