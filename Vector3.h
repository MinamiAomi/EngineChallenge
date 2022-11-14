#pragma once
#include "Vector2.h"

// �����^���R�ێ�����N���X�iVector2�̐e�j
template<class TYPE>
class Triplet {

public:
	TYPE x;	// x����
	TYPE y; // y����
	TYPE z; // z����

public:
	Triplet() {}
	Triplet(const TYPE& x, const TYPE& y, const TYPE& z) : x(x), y(y), z(z) {}
	Triplet(const Triplet<TYPE>& t) : x(t.x), y(t.y), z(t.z) {}
	Triplet(const Twins<TYPE>& t, const TYPE& z) : x(t.x), y(t.y), z(z) {}
	virtual ~Triplet() {}
	// �I�[�o�[���[�h

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

// 3D�x�N�g��
class Vector3 :
	public Triplet<float> {

public:
	Vector3() : Triplet(0.0f, 0.0f, 0.0f) {}
	Vector3(float x, float y, float z) : Triplet(x, y, z) {}
	Vector3(const Vector3& v) : Triplet(v.x, v.y, v.z) {}
	Vector3(const Vector2& v2, float z) : Triplet(v2, z) {}
	~Vector3() {}

};