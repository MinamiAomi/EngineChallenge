#include "Vector3.h"

// ゼロベクトル
const Vector3 Vector3::Zero = { 0.0f,0.0f,0.0f };
// X軸向きの単位ベクトル(1,0,0)
const Vector3 Vector3::UnitX = { 1.0f ,0.0f ,0.0f };
// Y軸向きの単位ベクトル(0,1,0)
const Vector3 Vector3::UnitY = { 0.0f ,1.0f ,0.0f };
// Z軸向きの単位ベクトル(0,0,1)
const Vector3 Vector3::UnitZ = { 0.0f ,0.0f ,1.0f };