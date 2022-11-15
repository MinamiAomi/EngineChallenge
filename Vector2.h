#pragma once

// �����^���Q�ێ�����N���X�iVector2�̐e�j
template<class TYPE>
class Twins {

public:
	TYPE x;	// x����
	TYPE y; // y����


public:
	Twins() {}
	Twins(const TYPE& x, const TYPE& y) : x(x), y(y) {}
	Twins(const Twins& p) : x(p.x), y(p.y) {}
	virtual ~Twins() {}
	// �I�[�o�[���[�h

	inline void Set(const TYPE& sx, const TYPE& sy) { x = sx, y = sy; }
	inline void Set(const Twins& p) { x = p.x, y = p.y; }

	inline Twins operator+() const { return { x, y }; }
	inline Twins operator-() const { return { -x,-y }; }
	inline Twins& operator+=(const Twins& p) {
		x += p.x;
		y += p.y;
		return *this;
	}
	inline Twins& operator-=(const Twins& p) {
		x -= p.x;
		y -= p.y;
		return *this;
	}
	inline Twins& operator*=(const TYPE& s) {
		x *= s;
		y *= s;
		return *this;
	}
	inline Twins& operator/=(const TYPE& s) {
		x /= s;
		y /= s;
		return *this;
	}
};

// 2D�x�N�g��
class Vector2 
{
public:

	float x;	// x����
	float y; // y����

public:
	Vector2() : x(0.0f), y(0.0f) {}
	Vector2(float x, float y) : x(x), y(y) {}
	Vector2(const Vector2& v) : x(v.x), y(v.y) {}
	~Vector2() {}
};