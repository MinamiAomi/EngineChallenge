#pragma once

// 同じ型を２つ保持するクラス（Vector2の親）
template<class TYPE>
class Twins {

public:
	TYPE x;	// x成分
	TYPE y; // y成分

public:
	Twins() {}
	Twins(const TYPE& x, const TYPE& y) : x(x), y(y) {}
	Twins(const Twins& p) : x(p.x), y(p.y) {}
	virtual ~Twins() {}
	// オーバーロード

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

// 2Dベクトル
class Vector2 :
	public Twins<float> {

public:
	Vector2() : Twins(0.0f, 0.0f) {}
	Vector2(float x, float y) : Twins(x, y) {}
	Vector2(const Vector2& v) : Twins(v.x, v.y) {}
	~Vector2() {}
};