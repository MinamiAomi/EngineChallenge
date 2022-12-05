#include "MathUtility.h"

unsigned int Color::Create(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
	return r << 24 | g << 16 | b << 8 | a;
}

Vector4 Color::ToVector4(unsigned int color)
{
	Vector4 result = {
		static_cast<float>((color >> 24) & 0xFF) / 255.0f,
		static_cast<float>((color >> 16) & 0xFF) / 255.0f,
		static_cast<float>((color >> 8) & 0xFF) / 255.0f,
		static_cast<float>((color >> 0) & 0xFF) / 255.0f
	};
	static auto toLinear = [](float color) {
		if (0 <= color && color <= 0.04045f) {
			return color / 12.92f;
		}
		return Math::Pow((color + 0.055f) / 1.055f, 2.4f);
	};

	result.x = toLinear(result.x);
	result.y = toLinear(result.y);
	result.z = toLinear(result.z);

	return result;
}

unsigned int HsvaToRgba(float h, float s, float v, unsigned int a) 
{
	Vector4 rgba;

	float H = fmodf(h, 360.0f);
	float V = Math::Clamp(v, 0.0f, 1.0f);
	float S = Math::Clamp(s, 0.0f, 1.0f);;
	if (S <= 0) {
		rgba.xyz(v, v, v);
	}
	else {

		int Hi = (int)(H / 60.0f);
		float F = H / 60.0f - (float)Hi;
		float M = V * (1.0f - S);
		float N = V * (1.0f - S * F);
		float K = V * (1.0f - S * (1.0f - F));

		switch (Hi)
		{
		case 0:
			rgba.xyz(V, K, M);
			break;
		case 1:
			rgba.xyz(N, V, M);
			break;
		case 2:
			rgba.xyz(M, V, K);
			break;
		case 3:
			rgba.xyz(M, N, V);
			break;
		case 4:
			rgba.xyz(K, M, V);
			break;
		case 5:
		default:
			rgba.xyz(V, M, N);
			break;
		}
	}
	return Color::Create((unsigned int)rgba.x, (unsigned int)rgba.y, (unsigned int)rgba.z, a);
}