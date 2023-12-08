#include "ext_matrix.h"

using namespace ext;

vec2d<float> ext::operator*(const Matrix<3, 3>& mat, const vec2d<float>& vec)
{
	return {
		mat[0][0] * vec.x + mat[0][1] * vec.y + mat[0][2],
		mat[1][0] * vec.x + mat[1][1] * vec.y + mat[1][2]
	};
}
Matrix<3, 3> ext::Mat3x3_Rotate(float fAngle)
{
	return {
		cosf(fAngle), -sinf(fAngle), 0.0f,
		sinf(fAngle), cosf(fAngle), 0.0f,
		0.0f, 0.0f, 1.0f
	};
}
Matrix<3, 3> ext::Mat3x3_Scale(vec2d<float> scale)
{
	return {
		scale.x, 0.0f, 0.0f,
		0.0f, scale.y, 0.0f,
		0.0f, 0.0f, 1.0f
	};
}
Matrix<3, 3> ext::Mat3x3_Translate(vec2d<float> v)
{
	return {
		1.0f, 0.0f, v.x,
		0.0f, 1.0f, v.y,
		0.0f, 0.0f, 1.0f
	};
}

vec3d<float> ext::operator*(const Matrix<4, 4>& mat, const vec3d<float>& v)
{
	Matrix<4, 1> r = mat * Matrix<4, 1>{v.x, v.y, v.z, 1.0f};
	return { r[0][0],r[1][0],r[2][0] };
}
vec4d<float> ext::operator*(const Matrix<4, 4>& mat, const vec4d<float>& v)
{
	Matrix<4, 1> r = mat * Matrix<4, 1>{v.x, v.y, v.z, v.w};
	return { r[0][0],r[1][0],r[2][0],r[3][0] };
}
Matrix<4, 4> ext::Mat4x4_RotateY(float t)
{
	return {
		cosf(t)    ,0.0f       ,sinf(t)    ,0.0f       ,
		0.0f       ,1.0f       ,0.0f       ,0.0f       ,
		-sinf(t)   ,0.0f       ,cosf(t)    ,0.0f       ,
		0.0f       ,0.0f       ,0.0f       ,1.0f
	};
}
Matrix<4, 4> ext::Mat4x4_RotateX(float t)
{
	return {
		1.0f       ,0.0f       ,0.0f       ,0.0f       ,
		0.0f       ,cosf(t)    ,-sinf(t)   ,0.0f       ,
		0.0f       ,sinf(t)    ,cosf(t)    ,0.0f       ,
		0.0f       ,0.0f       ,0.0f       ,1.0f
	};
}
Matrix<4, 4> ext::Mat4x4_RotateZ(float t)
{
	return {
		cosf(t)    ,-sinf(t)   ,0.0f       ,0.0f       ,
		sinf(t)    ,cosf(t)    ,0.0f       ,0.0f       ,
		0.0f       ,0.0f       ,1.0f       ,0.0f       ,
		0.0f       ,0.0f       ,0.0f       ,1.0f
	};
}
Matrix<4, 4> ext::Mat4x4_RotateY(float cos, float sin)
{
	return {
		cos    ,0.0f       ,sin    ,0.0f       ,
		0.0f       ,1.0f       ,0.0f       ,0.0f       ,
		-sin   ,0.0f       ,cos    ,0.0f       ,
		0.0f       ,0.0f       ,0.0f       ,1.0f
	};
}
Matrix<4, 4> ext::Mat4x4_RotateX(float cos, float sin)
{
	return {
		1.0f       ,0.0f       ,0.0f       ,0.0f       ,
		0.0f       ,cos    ,-sin   ,0.0f       ,
		0.0f       ,sin    ,cos    ,0.0f       ,
		0.0f       ,0.0f       ,0.0f       ,1.0f
	};
}
Matrix<4, 4> ext::Mat4x4_RotateZ(float cos, float sin)
{
	return {
		cos    ,-sin   ,0.0f       ,0.0f       ,
		sin    ,cos    ,0.0f       ,0.0f       ,
		0.0f       ,0.0f       ,1.0f       ,0.0f       ,
		0.0f       ,0.0f       ,0.0f       ,1.0f
	};
}
Matrix<4, 4> ext::Mat4x4_Scale(const vec3d<float>& v)
{
	return {
		v.x        ,0.0f       ,0.0f       ,0.0f       ,
		0.0f       ,v.y        ,0.0f       ,0.0f       ,
		0.0f       ,0.0f       ,v.z        ,0.0f       ,
		0.0f       ,0.0f       ,0.0f       ,1.0f
	};
}
Matrix<4, 4> ext::Mat4x4_Translate(const vec3d<float>& v)
{
	return {
	1.0f       ,0.0f       ,0.0f       ,v.x        ,
	0.0f       ,1.0f       ,0.0f       ,v.y        ,
	0.0f       ,0.0f       ,1.0f       ,v.z        ,
	0.0f       ,0.0f       ,0.0f       ,1.0f
	};
}

float ext::Mat_Det(const Matrix<1, 1>& mat)
{
	return mat[0][0];
}
float ext::Mat_Det(const Matrix<2, 2>& mat)
{
	return mat[0][0] * mat[1][1] - mat[0][1] * mat[1][0];
}
float ext::Mat_Det(const Matrix<3, 3>& mat)
{
	float det = 0.0f;
	for (int i = 0; i < 3; i++)
	{
		det +=
			mat[0][i] * mat[1][(i + 1) % 3] * mat[2][(i + 2) % 3] -
			mat[0][i] * mat[1][(i + 2) % 3] * mat[2][(i + 1) % 3];
	}
	return det;
}