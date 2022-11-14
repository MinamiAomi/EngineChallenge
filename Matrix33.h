#pragma once
#include "Vector2.h"
#include <math.h>
#include <cassert>

class Matrix33 
{

private:
	float m[3][3];

public:
	Matrix33(float _00 = 1.0f, float _01 = 0.0f, float _02 = 0.0f,
			 float _10 = 0.0f, float _11 = 1.0f, float _12 = 0.0f,
			 float _20 = 0.0f, float _21 = 0.0f, float _22 = 1.0f) {
		m[0][0] = _00, m[0][1] = _01, m[0][2] = _02;
		m[1][0] = _10, m[1][1] = _11, m[1][2] = _12;
		m[2][0] = _20, m[2][1] = _21, m[2][2] = _22;
	}

	const float* operator[](size_t i) const {
		return m[i];
	}

	friend inline Matrix33 operator*(const Matrix33& mat1, const Matrix33& mat2) {
		return {
			mat1.m[0][0] * mat2.m[0][0] + mat1.m[0][1] * mat2.m[1][0] + mat1.m[0][2] * mat2.m[2][0],
			mat1.m[0][0] * mat2.m[0][1] + mat1.m[0][1] * mat2.m[1][1] + mat1.m[0][2] * mat2.m[2][1],
			mat1.m[0][0] * mat2.m[0][2] + mat1.m[0][1] * mat2.m[1][2] + mat1.m[0][2] * mat2.m[2][2],

			mat1.m[1][0] * mat2.m[0][0] + mat1.m[1][1] * mat2.m[1][0] + mat1.m[1][2] * mat2.m[2][0],
			mat1.m[1][0] * mat2.m[0][1] + mat1.m[1][1] * mat2.m[1][1] + mat1.m[1][2] * mat2.m[2][1],
			mat1.m[1][0] * mat2.m[0][2] + mat1.m[1][1] * mat2.m[1][2] + mat1.m[1][2] * mat2.m[2][2],

			mat1.m[2][0] * mat2.m[0][0] + mat1.m[2][1] * mat2.m[1][0] + mat1.m[2][2] * mat2.m[2][0],
			mat1.m[2][0] * mat2.m[0][1] + mat1.m[2][1] * mat2.m[1][1] + mat1.m[2][2] * mat2.m[2][1],
			mat1.m[2][0] * mat2.m[0][2] + mat1.m[2][1] * mat2.m[1][2] + mat1.m[2][2] * mat2.m[2][2]
		};
	}

	inline Matrix33& operator*=(const Matrix33& mat) {
		*this = *this * mat;
		return *this;
	}

	friend inline Vector2 operator*(const Vector2& vec, const Matrix33& mat) {
		return {
			vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + 1.0f * mat.m[2][0],
			vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + 1.0f * mat.m[2][1]
		};
	}

	static inline Vector2 MultiplyVector2(const Vector2& vec, const Matrix33& mat) {
		return vec * mat;
	}

	friend inline Matrix33 operator*(float s, const Matrix33& mat) {
		return {
			s * mat.m[0][0], s * mat.m[0][1], s * mat.m[0][2],
			s * mat.m[1][0], s * mat.m[1][1], s * mat.m[1][2],
			s * mat.m[2][0], s * mat.m[2][1], s * mat.m[2][2] };
	}
	friend inline Matrix33 operator*(const Matrix33& mat, float s) {
		return {
			mat.m[0][0] * s, mat.m[0][1] * s, mat.m[0][2] * s,
			mat.m[1][0] * s, mat.m[1][1] * s, mat.m[1][2] * s,
			mat.m[2][0] * s, mat.m[2][1] * s, mat.m[2][2] * s };
	}

	float Determinant() const {
		return m[0][0] * m[1][1] * m[2][2] + m[0][1] * m[1][2] * m[2][0] + m[0][2] * m[1][0] * m[2][1]
			 - m[0][2] * m[1][1] * m[2][0] - m[0][1] * m[1][0] * m[2][2] - m[0][0] * m[1][2] * m[2][1];
	}

	Matrix33 Adjugate() const {
		return {
			  m[1][1] * m[2][2] - m[1][2] * m[2][1],
			-(m[0][1] * m[2][2] - m[0][2] * m[2][1]),
			  m[0][1] * m[1][2] - m[0][2] * m[1][1],

			-(m[1][0] * m[2][2] - m[1][2] * m[2][0]),
			  m[0][0] * m[2][2] - m[0][2] * m[2][0],
			-(m[0][0] * m[1][2] - m[0][2] * m[1][0]),

			  m[1][0] * m[2][1] - m[1][1] * m[2][0],
			-(m[0][0] * m[2][1] - m[0][1] * m[2][0]),
			  m[0][0] * m[1][1] - m[0][1] * m[1][0] };
	}

	Matrix33 Inverse() const {
		float det = Determinant();
		assert(det != 0.0f); // 0‚Ìê‡‹ts—ñ‚ª‚È‚¢
		return 1.0f / det * Adjugate();
	}

	// ’PˆÊs—ñ
	static inline Matrix33 CreateIdentity() {
		return {
			1.0f,		0.0f,		0.0f,
			0.0f,		1.0f,		0.0f,
			0.0f,		0.0f,		1.0f
		};
	}
	// CreateŠÖ”
		// Šg‘åk¬s—ñ
	static inline Matrix33 CreateScaling(float scaleX, float scaleY) {
		return {
			scaleX, 0.0f,		0.0f,
			0.0f,		scaleY,	0.0f,
			0.0f,		0.0f,	1.0f
		};
	}
	// Šg‘åk¬s—ñ
	static inline Matrix33 CreateScaling(float scale) {
		return CreateScaling(scale, scale);
	}
	// Šg‘åk¬s—ñ
	static inline Matrix33 CreateScaling(const Vector2& scale) {
		return CreateScaling(scale.x, scale.y);
	}
	// ‰ñ“]s—ñ
	static inline Matrix33 CreateRotation(float theta) {
		float s = sinf(theta);
		float c = cosf(theta);
		return {
			c,		s,		0.0f,
			-s,		c,		0.0f,
			0.0f,	0.0f,	1.0f
		};
	}
	// •½sˆÚ“®s—ñ
	static inline Matrix33 CreateTranslation(float vecX, float vecY) {
		return {
			1.0f,	0.0f,	0.0f,
			0.0f,	1.0f,	0.0f,
			vecX,	vecY,	1.0f
		};
	}
	// •½sˆÚ“®s—ñ
	static inline Matrix33 CreateTranslation(const Vector2& vector) {
		return CreateTranslation(vector.x, vector.y);
	}
	// ƒAƒtƒBƒ“•ÏŠ·s—ñ
	static inline Matrix33 CreateAffine(float scaleX, float scaleY, float theta, float vecX, float vecY) {
		float s = sinf(theta);
		float c = cosf(theta);
		return {
			scaleX * c,		scaleX * s,		0.0f,
			scaleY * -s,	scaleY * c,		0.0f,
			vecX,			vecY,			1.0f
		};
	}
	// ƒAƒtƒBƒ“•ÏŠ·s—ñ
	static inline Matrix33 CreateAffine(float scale, float theta, const Vector2& vec) {
		return CreateAffine(scale, scale, theta, vec.x, vec.y);
	}
	// ƒAƒtƒBƒ“•ÏŠ·s—ñ
	static inline Matrix33 CreateAffine(const Vector2& scale, float theta, const Vector2& vec) {
		return CreateAffine(scale.x, scale.y, theta, vec.x, vec.y);
	}

	static inline Matrix33 CreateInverse(const Matrix33& matrix) {
		return matrix.Inverse();
	}

	void SetIdentity() {
		*this = CreateIdentity();
	}

	// •ÏŠ·s—ñ‚Æ‚ÌŠ|‚¯Z(Œø—¦“I)
		// Šg‘åk¬s—ñ‚Æ‚ÌŠ|‚¯Z
	void inline MultiplyScaling(float scaleX, float scaleY) {
		// |  _00  _01  0  |   |  X  0  0  |   |  _00X  _01Y  0  |
		// |  _10  _11  0  | * |  0  Y  0  | = |  _10X  _11Y  0  |
		// |  _20  _21  1  |   |  0  0  1  |   |  _20   _21   1  |
		m[0][0] *= scaleX; m[0][1] *= scaleY;
		m[1][0] *= scaleX; m[1][1] *= scaleY;
	}
	// Šg‘åk¬s—ñ‚Æ‚ÌŠ|‚¯Z
	void inline MultiplyScaling(float scale) {
		MultiplyScaling(scale, scale);
	}
	// Šg‘åk¬s—ñ‚Æ‚ÌŠ|‚¯Z
	void inline MultiplyScaling(const Vector2& scale) {
		MultiplyScaling(scale.x, scale.y);
	}
	// ‰ñ“]s—ñ‚Æ‚ÌŠ|‚¯Z
	void inline MultiplyRotation(float theta) {
		// |  _00  _01  0  |   |   c  s  0  |   | _00c+_01(-s)  _00s+_01c  0  |
		// |  _10  _11  0  | * |  -s  c  0  | = | _10c+_11(-s)  _10s+_11c  0  |
		// |  _20  _21  1  |   |   0  0  1  |   |          _20        _21  1  |
		float s = sinf(theta);
		float c = cosf(theta);
		float tmp;
		tmp = m[0][0] * c + m[0][1] * -s;
		m[0][1] = m[0][0] * s + m[0][1] * c;
		m[0][0] = tmp;

		tmp = m[1][0] * c + m[1][1] * -s;
		m[1][1] = m[1][0] * s + m[1][1] * c;
		m[1][0] = tmp;
	}
	// •½sˆÚ“®s—ñ‚Æ‚ÌŠ|‚¯Z
	void inline MultiplyTranslation(float vecX, float vecY) {
		// |  _00  _01  0  |   |  1  0  0  |   | _00    _01    0  |
		// |  _10  _11  0  | * |  0  1  0  | = | _10	_11    0  |
		// |  _20  _21  1  |   |  X  Y  1  |   | _20+X  _21+Y  1  |
		m[2][0] += vecX;
		m[2][1] += vecY;
	}
	// •½sˆÚ“®s—ñ‚Æ‚ÌŠ|‚¯Z
	void inline MultiplyTranslation(const Vector2& vector) {
		MultiplyTranslation(vector.x, vector.y);
	}

};