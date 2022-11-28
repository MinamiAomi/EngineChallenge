#pragma once
#include "Vector3.h"
#include "Matrix44.h"

class Quaternion 
{

public:
	float x;
	float y;
	float z;
	float w;

public:
	Quaternion(float x = 0.0f, float y = 0.0f, float z = 0.0f, float w = 1.0f) :
		x(x), y(y), z(z), w(w) {}
	Quaternion(const Vector3& v, float a) {
		float sin2 = sinf(a / 2.0f);
		x = v.x * sin2;
		y = v.y * sin2;
		z = v.z * sin2;
		w = cosf(a / 2.0f);
	}

	static Quaternion Create(const Vector3& v, float a) {
		Quaternion tmp(v, a);
		return tmp;
	}

	friend inline Quaternion operator*(float s, const Quaternion& q) {
		return { s * q.x,s * q.y,s * q.z,s * q.w };
	}
	friend inline Quaternion operator*(const Quaternion& q, float s) {
		return { q.x * s, q.y * s, q.z * s, q.w * s };
	}
	friend inline Quaternion operator/(const Quaternion& q, float s) {
		return { q.x / s, q.y / s, q.z / s, q.w / s };
	}
	friend inline Quaternion operator*(const Quaternion& q1, const Quaternion& q2) {
		Vector3 v1(q1.x, q1.y, q1.z);
		Vector3 v2(q2.x, q2.y, q2.z);
		Vector3 v = Cross(v1, v2) + q2.w * v1 + q1.w * v2;
		return { v.x, v.y, v.z, q1.w * q2.w - v1.Dot(v2) };
	}
	static inline Vector3 TransForm(const Vector3& r, const Quaternion& q) {
		Vector3 v(q.x, q.y, q.z);
		return r + 2.0f * Cross(v, Cross(v, r) + q.w * r);
	}
	inline float Dot(const Quaternion& q) const {
		return x * q.x + y * q.y + z * q.z + w * q.w;
	}
	inline float LengthSquare() const {
		return Dot(*this);
	}
	inline float Length() const {
		return sqrtf(LengthSquare());
	}
	inline Quaternion Normalized() const {
		float len = Length();
		return *this / len;
	}
	inline void Set(float ix, float iy, float iz, float iw) {
		x = ix, y = iy, z = iz, w = iw;
	}
	inline Vector3 VectorPart() const {
		return { x,y,z };
	}
	inline float GetAngle() const {
		return acosf(w) * 2.0f;
	}
	inline Vector3 GetAxis() const { 
		return VectorPart() / sinf(acosf(w));
	}

	inline Quaternion Conjugate() const {
		return { -x, -y, -z, w };
	}
	inline Quaternion Inverse() const {
		return Conjugate() / LengthSquare();
	}

};