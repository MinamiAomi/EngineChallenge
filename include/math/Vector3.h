#pragma once

// �����^���Q�ێ�����N���X�iVector2�̐e�j
template<class TYPE>
class Trio {

public:
	TYPE x;	// x����
	TYPE y; // y����
	TYPE z; // z����

public:
	Trio() {}
	Trio(const TYPE& x, const TYPE& y, const TYPE& z) : x(x), y(y), z(z) {}
	Trio(const Trio<TYPE>& t) : x(t.x), y(t.y), z(t.z) {}
	virtual ~Trio() {}
	// �I�[�o�[���[�h

	inline void Set(const TYPE& sx, const TYPE& sy, const TYPE& sz) { x = sx, y = sy, z = sz; }
	inline void Set(const Trio& t) { x = t.x, y = t.y, z = t.z; }

	inline Trio operator+() const { return { x, y, z }; }
	inline Trio operator-() const { return { -x,-y, -z }; }
	inline Trio& operator+=(const Trio& t) {
		x += t.x;
		y += t.y;
		z += t.z;
		return *this;
	}
	inline Trio& operator-=(const Trio& t) {
		x -= t.x;
		y -= t.y;
		z -= t.z;
		return *this;
	}
	inline Trio& operator*=(const TYPE& s) {
		x *= s;
		y *= s;
		z *= s;
		return *this;
	}
	inline Trio& operator/=(const TYPE& s) {
		x /= s;
		y /= s;
		z /= s;
		return *this;
	}
};

// 2D�x�N�g��
class Vector3 :
	public Trio<float> {

public:
	Vector3() : Trio(0.0f, 0.0f, 0.0f) {}
	Vector3(float x, float y, float z) : Trio(x, y, z) {}
	Vector3(const Vector3& v) : Trio(v.x, v.y, v.z) {}
	~Vector3() {}
};