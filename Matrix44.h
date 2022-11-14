#pragma once
#include "Vector3.h"
#include <math.h>

class Matrix44
{

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



	static inline Matrix44 MakeIdentity() {
		return{ 1.0f, 0.0f, 0.0f, 0.0f,
				0.0f, 1.0f, 0.0f, 0.0f,
				0.0f, 0.0f, 1.0f, 0.0f,
				0.0f, 0.0f, 0.0f, 1.0f };
	}

	static inline Matrix44 MakeScaling(float scaleX, float scaleY, float scaleZ) {
		return{ scaleX,		0.0f,		0.0f,		0.0f,
				0.0f,		scaleY,		0.0f,		0.0f,
				0.0f,		0.0f,		scaleZ,		0.0f,
				0.0f,		0.0f,		0.0f,		1.0f };
	}
	static inline Matrix44 MakeScaling(float scale) {
		return MakeScaling(scale, scale, scale);
	};
	static inline Matrix44 MakeScaling(const Vector3& scale) {
		return MakeScaling(scale.x, scale.y, scale.z);
	}
	
	static inline Matrix44 MakeRotationX(float theta) {
		float c = cosf(theta);
		float s = sinf(theta);
		return{ 1.0f,		0.0f,		0.0f,		0.0f,
				0.0f,		c,			s,			0.0f,
				0.0f,		-s,			c,			0.0f,
				0.0f,		0.0f,		0,			1.0f };
	}
	static inline Matrix44 MakeRotationY(float theta) {
		float c = cosf(theta);
		float s = sinf(theta);
		return{ c,			0.0f,		-s,			0.0f,
				0.0f,		1.0f,		0.0f,		0.0f,
				s,			0.0f,		c,			0.0f,
				0.0f,		0.0f,		0,			1.0f };
	}
	static inline Matrix44 MakeRotationZ(float theta) {
		float c = cosf(theta);
		float s = sinf(theta);
		return{ c,			s,			0.0f,		0.0f,
				-s,			c,			0.0f,		0.0f,
				0.0f,		0.0f,		1.0f,		0.0f,
				0.0f,		0.0f,		0,			1.0f };
	}

	static inline Matrix44 MakeTranslation(float x, float y, float z) {
		return{ 1.0f,	0.0f,	0.0f,	0.0f,
				0.0f,	1.0f,	0.0f,	0.0f,
				0.0f,	0.0f,	1.0f,	0.0f,
				x,		y,		z,		1.0f };
	}
	static inline Matrix44 MakeTranslation(const Vector3& trans) {
		return MakeTranslation(trans.x, trans.y, trans.z);
	}


};
