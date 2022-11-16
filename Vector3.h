#pragma once
#ifndef VECTOR3_H_
#define VECTOR3_H_

#include "Vector2.h"
#include <cassert>

// 同じ型を３つ保持するクラス
template<class TYPE>
class Triplet {

public:
	TYPE x;	// x成分
	TYPE y; // y成分
	TYPE z; // z成分

public:
	Triplet() {}
	Triplet(const TYPE& x, const TYPE& y, const TYPE& z) : x(x), y(y), z(z) {}
	Triplet(const Triplet<TYPE>& t) : x(t.x), y(t.y), z(t.z) {}
	Triplet(const Twins<TYPE>& t, const TYPE& z) : x(t.x), y(t.y), z(z) {}
	virtual ~Triplet() {}
	// オーバーロード

	inline void Set(const TYPE& sx, const TYPE& sy, const TYPE& sz) { x = sx, y = sy, z = sz; }
	inline void Set(const Triplet& t) { x = t.x, y = t.y, z = t.z; }
	inline void Set(const Twins<TYPE>& t, const TYPE& z) { x = t.x, y = t.y, z = z; }

	inline Triplet operator+() const { return { x, y, z }; }
	inline Triplet operator-() const { return { -x,-y, -z }; }
	inline Triplet& operator+=(const Triplet& t) {
		x += t.x;
		y += t.y;
		z += t.z;
		return *this;
	}
	inline Triplet& operator-=(const Triplet& t) {
		x -= t.x;
		y -= t.y;
		z -= t.z;
		return *this;
	}
	inline Triplet& operator*=(const TYPE& s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	inline Triplet& operator/=(const TYPE& s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}
};

// 3Dベクトル
class Vector3
{
public:
	static const Vector3 Zero;
	static const Vector3 UnitX;	
	static const Vector3 UnitY;
	static const Vector3 UnitZ;

public:
	float x;	// x成分
	float y; // y成分
	float z; // z成分

public:
	Vector3() : x(0.0f), y(0.0f), z(0.0f) {}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
	Vector3(const Vector3& v) : x(v.x), y(v.y), z(v.z) {}
	Vector3(const Vector2& v2, float z) : x(v2.x), y(v2.y), z(z) {}
	~Vector3() {}

	inline Vector3 operator+() const { return { x, y, z }; }
	inline Vector3 operator-() const { return { -x,-y, -z }; }
	inline Vector3& operator+=(const Vector3& t) {
		x += t.x;
		y += t.y;
		z += t.z;
		return *this;
	}
	inline Vector3& operator-=(const Vector3& t) {
		x -= t.x;
		y -= t.y;
		z -= t.z;
		return *this;
	}
	inline Vector3& operator*=(float s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	inline Vector3& operator/=(float s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}

	friend inline Vector3 operator+(const Vector3& a, const Vector3& b) {
		return { a.x + b.x, a.y + b.y, a.z + b.z };
	}
	friend inline Vector3 operator-(const Vector3& a, const Vector3& b) {
		return { a.x - b.x, a.y - b.y, a.z - b.z };
	}
	friend inline Vector3 operator*(const Vector3& a, float s) {
		return { a.x * s, a.y * s, a.z * s };
	}
	friend inline Vector3 operator*(float s, const Vector3& a) {
		return { s * a.x, s * a.y, s * a.z };
	}
	friend inline Vector3 operator/(const Vector3& a, float s) {
		return { a.x / s, a.y / s, a.z / s };
	}

	inline float Dot(const Vector3& vec) const {
		return x * vec.x + y * vec.y + z * vec.z;
	}

	inline Vector3 Cross(const Vector3& vec) const {
		return { y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x };
	}
	// 長さの二乗
	inline float LengthSquare() const {
		return Dot(*this);
	}

	inline float Length() const {
		return sqrtf(LengthSquare());
	}

	// 他ベクトルとの距離
	inline float Distance(const  Vector3& v) const {
		return (v - *this).Length();
	}

	inline Vector3 Normalized() const {
		float len = Length();
		assert(len != 0);
		return *this / len;
	}

	// 中点
	inline Vector3 Mid(const Vector3& v) const {
		return (*this + v) / 2.0f;
	}

	// 線形補間
	static inline  Vector3 Lerp(float t, const  Vector3& start, const  Vector3& end) {
		return start + t * (end - start);
	}
}; 

inline float Dot(const Vector3& a, const Vector3& b) {
	return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3 Cross(const Vector3& a, const Vector3& b) {
	return { a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x };
}

inline float LengthSquare(const Vector3& a)  {
	return Dot(a, a);
}

inline float Length(const Vector3& a) {
	return sqrtf(LengthSquare(a));
}

inline Vector3 Normalize(const Vector3& a) {
	float len = Length(a);
	return len != 0 ? a / len : a;
}

// 中点
inline Vector3 Mid(const Vector3& a, const Vector3& b) {
	return (a + b) / 2.0f;
}

// 線形補間
inline  Vector3 Lerp(float t, const  Vector3& start, const  Vector3& end) {
	return start + t * (end - start);
}



#endif