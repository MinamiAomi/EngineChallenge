#include "Matrix44.h"
#include "Quaternion.h"

const Matrix44 Matrix44::Identity = { 
	1.0f,0.0f,0.0f,0.0f, 
	0.0f,1.0f,0.0f,0.0f, 
	0.0f,0.0f,1.0f,0.0f, 
	0.0f,0.0f,0.0f,1.0f };

Matrix44 Matrix44::CreateRotation(const Quaternion& q) 
{
	Matrix44 tmp;
	tmp.m[0][0] = 1.0f - 2.0f*q.y*q.y - 2.0f*q.z*q.z;	// 1 - 2*y*y - 2*z*z
	tmp.m[0][1] = 2.0f*q.x*q.y + 2.0f*q.w*q.z;			// 2*x*y + 2*w*z
	tmp.m[0][2] = 2.0f*q.x*q.z - 2.0f*q.w*q.y;			// 2*x*z - 2*w*y
	
	tmp.m[1][0] = 2.0f*q.x*q.y - 2.0f*q.w*q.z;			// 2*x*y - 2*w*z
	tmp.m[1][1] = 1.0f - 2.0f*q.x*q.x - 2.0f*q.z*q.z;	// 1 - 2*x*x - 2*z*z
	tmp.m[1][2] = 2.0f*q.y*q.z + 2.0f*q.w*q.x;			// 2*y*z + 2*w*x
	
	tmp.m[2][0] = 2.0f*q.x*q.z + 2.0f*q.w*q.y;			// 2*x*z + 2*w*y
	tmp.m[2][1] = 2.0f*q.y*q.z - 2.0f*q.w*q.x;			// 2*y*z - 2*w*x
	tmp.m[2][2] = 1.0f - 2.0f*q.x*q.x - 2.0f*q.y*q.y;	// 1 - 2*x*x - 2*y*y

	//tmp.m[0][0] = q.w*q.w + q.x*q.x - q.y*q.y - q.z*q.z;
	//tmp.m[0][1] = 2.0f * (q.x*q.y - q.w*q.z);
	//tmp.m[0][2] = 2.0f * (q.x*q.z + q.w*q.y);
	//
	//tmp.m[1][0] = 2.0f * (q.x*q.y + q.w*q.z);
	//tmp.m[1][1] = q.w*q.w - q.x*q.x + q.y*q.y - q.z*q.z;
	//tmp.m[1][2] = 2.0f * (q.y*q.z - q.w*q.x);
	//
	//tmp.m[2][2] = 2.0f * (q.x*q.z + q.w*q.y);
	//tmp.m[2][2] = 2.0f * (q.x * q.z + q.w * q.y);
	//tmp.m[1][1] = q.w*q.w - q.x*q.x - q.y*q.y + q.z*q.z;

	return tmp;
}

Matrix44 Matrix44::CreateRotationFromQuaternion(const Quaternion& q) 
{
	return CreateRotation(q);
}