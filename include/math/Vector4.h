#pragma once

// �����^���S�ێ�����N���X�iVector2�̐e�j
template<class TYPE>
class Quadruplet {

public:
	TYPE x;	// x����
	TYPE y; // y����
	TYPE z; // z����
	TYPE w; // w����

public:
	Quadruplet() {}
	Quadruplet(const TYPE& x, const TYPE& y, const TYPE& z, const TYPE& w) : x(x), y(y), z(z), w(w) {}
	Quadruplet(const Quadruplet<TYPE>& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
	virtual ~Quadruplet() {}
	// �I�[�o�[���[�h

	inline void Set(const TYPE& sx, const TYPE& sy, const TYPE& sz, const TYPE& sw)
	{
		x = sx, y = sy, z = sz, w = sw;
	}
	inline void Set(const Quadruplet& q) { x = q.x, y = q.y, z = q.z, w = q.w; }

	inline Quadruplet operator+() const { return { x, y, z, w }; }
	inline Quadruplet operator-() const { return { -x,-y, -z, -w }; }
	inline Quadruplet& operator+=(const Quadruplet& q) {
		x += q.x;
		y += q.y;
		z += q.z;
		w += q.w;
		return *this;
	}
	inline Quadruplet& operator-=(const Quadruplet& q) {
		x -= q.x;
		y -= q.y;
		z -= q.z;
		w -= q.w;
		return *this;
	}
	inline Quadruplet& operator*=(const TYPE& s) {
		x *= s;
		y *= s;
		z *= s;
		w *= s;
		return *this;
	}
	inline Quadruplet& operator/=(const TYPE& s) {
		x /= s;
		y /= s;
		z /= s;
		w /= s;
		return *this;
	}
};

// 4D�x�N�g��
class Vector4 :
	public Quadruplet<float> {

public:
	Vector4() : Quadruplet(0.0f, 0.0f, 0.0f, 0.0f) {}
	Vector4(float x, float y, float z, float w) : Quadruplet(x, y, z, w) {}
	Vector4(const Vector4& v) : Quadruplet(v.x, v.y, v.z, v.w) {}
	~Vector4() {}
};