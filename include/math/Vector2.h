#pragma once

// �����^���Q�ێ�����N���X�iVector2�̐e�j
template<class TYPE>
class Pair {

public:
	TYPE x;	// x����
	TYPE y; // y����

public:
	Pair() {}
	Pair(const TYPE& x, const TYPE& y) : x(x), y(y) {}
	Pair(const Pair& p) : x(p.x), y(p.y) {}
	virtual ~Pair() {}
	// �I�[�o�[���[�h

	inline void Set(const TYPE& sx, const TYPE& sy) { x = sx, y = sy; }
	inline void Set(const Pair& p) { x = p.x, y = p.y; }

	inline Pair operator+() const { return { x, y }; }
	inline Pair operator-() const { return { -x,-y }; }
	inline Pair& operator+=(const Pair& p) {
		x += p.x;
		y += p.y;
		return *this;
	}
	inline Pair& operator-=(const Pair& p) {
		x -= p.x;
		y -= p.y;
		return *this;
	}
	inline Pair& operator*=(const TYPE& s) {
		x *= s;
		y *= s;
		return *this;
	}
	inline Pair& operator/=(const TYPE& s) {
		x /= s;
		y /= s;
		return *this;
	}
};

// 2D�x�N�g��
class Vector2 :
	public Pair<float> {

public:
	Vector2() : Pair(0.0f, 0.0f) {}
	Vector2(float x, float y) : Pair(x, y) {}
	Vector2(const Vector2& v) : Pair(v.x, v.y) {}
	~Vector2() {}
};