#pragma once

// 同じ型を４つ保持するクラス（Vector2の親）
template<class TYPE>
class Quadruplet {

public:
	TYPE x;	// x成分
	TYPE y; // y成分
	TYPE z; // z成分
	TYPE w; // w成分

public:
	Quadruplet() {}
	Quadruplet(const TYPE& x, const TYPE& y, const TYPE& z, const TYPE& w) : x(x), y(y), z(z), w(w) {}
	Quadruplet(const Quadruplet<TYPE>& q) : x(q.x), y(q.y), z(q.z), w(q.w) {}
	virtual ~Quadruplet() {}
	// オーバーロード

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

// 4Dベクトル
class Vector4 :
	public Quadruplet<float> {

public:
	Vector4() : Quadruplet(0.0f, 0.0f, 0.0f, 0.0f) {}
	Vector4(float x, float y, float z, float w) : Quadruplet(x, y, z, w) {}
	Vector4(const Vector4& v) : Quadruplet(v.x, v.y, v.z, v.w) {}
	~Vector4() {}
};