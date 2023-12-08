#pragma once
#include "ext_vec2d.h"
#include "ext_vec3d.h"
#include "ext_vec4d.h"

namespace ext
{
	template <int I, int J>
	struct Matrix
	{
		float* operator[](int n)
		{
			return a[n];
		}
		const float* operator[](int n) const
		{
			return a[n];
		}
		float a[I][J] = { 0 };

		Matrix& operator*=(float rhs)
		{
			for (int i = 0; i < I; i++)
				for (int j = 0; j < J; j++)
					a[i][j] *= rhs;
			return *this;
		}
		Matrix& operator/=(float rhs)
		{
			for (int i = 0; i < I; i++)
				for (int j = 0; j < J; j++)
					a[i][j] /= rhs;
			return *this;
		}
		Matrix operator*(float rhs) const
		{
			return Matrix{ *this } *= rhs;
		}
		Matrix operator/(float rhs) const
		{
			return Matrix{ *this } /= rhs;
		}
	};

	vec2d<float> operator*(const Matrix<3, 3>& mat, const vec2d<float>& vec);
	Matrix<3, 3> Mat3x3_Rotate(float fAngle);
	Matrix<3, 3> Mat3x3_Scale(vec2d<float> scale);
	Matrix<3, 3> Mat3x3_Translate(vec2d<float> t);

	vec3d<float> operator*(const Matrix<4, 4>& mat, const vec3d<float>& vec);
	vec4d<float> operator*(const Matrix<4, 4>& mat, const vec4d<float>& vec);
	Matrix<4, 4> Mat4x4_RotateY(float t);
	Matrix<4, 4> Mat4x4_RotateX(float t);
	Matrix<4, 4> Mat4x4_RotateZ(float t);
	Matrix<4, 4> Mat4x4_RotateY(float cos, float sin);
	Matrix<4, 4> Mat4x4_RotateX(float cos, float sin);
	Matrix<4, 4> Mat4x4_RotateZ(float cos, float sin);
	Matrix<4, 4> Mat4x4_Scale(const vec3d<float>& v);
	Matrix<4, 4> Mat4x4_Translate(const vec3d<float>& v);

	float Mat_Det(const Matrix<1, 1>& mat);
	float Mat_Det(const Matrix<2, 2>& mat);
	float Mat_Det(const Matrix<3, 3>& mat);


	template <int N>
	Matrix<N, N> Mat_Identity()
	{
		Matrix<N, N> mat;
		for (int i = 0; i < N; i++)
			mat[i][i] = 1.0f;
		return mat;
	}
	template <int I, int J, int K>
	Matrix<I, J> operator*(const Matrix<I, K>& lhs, const Matrix<K, J>& rhs)
	{
		Matrix<I, J> mat;
		for (int i = 0; i < I; i++)
		{
			for (int j = 0; j < J; j++)
			{
				for (int k = 0; k < K; k++)
				{
					mat[i][j] += lhs[i][k] * rhs[k][j];
				}
			}
		}
		return mat;
	}
	template <int N>
	Matrix<N - 1, N - 1> Mat_Minor(const Matrix<N, N>& mat, int i_, int j_)
	{
		Matrix<N - 1, N - 1> minor;
		for (int i = 0, k = 0; i < N; i++)
		{
			if (i != i_)
			{
				for (int j = 0, l = 0; j < N; j++)
				{
					if (j != j_)
					{
						minor[k][l] = mat[i][j];
						l++;
					}
				}
				k++;
			}
		}
		return minor;
	}
	template <int N>
	float Mat_Det(const Matrix<N, N>& mat)
	{
		float det = 0.0f;
		for (int j = 0; j < N; j++)
		{
			det += mat[0][j] * float(((j + 1) % 2) * 2 - 1) * Mat_Det(Mat_Minor(mat, 0, j));
		}
		return det;
	}
	template <int N>
	Matrix<N, N> Mat_Inverse(const Matrix<N, N>& mat)
	{
		Matrix<N, N> Ct; //cofactor transposed (adjoint)
		for (int i = 0; i < N; i++)
		{
			for (int j = 0; j < N; j++)
			{
				Ct[j][i] = Mat_Det(Mat_Minor(mat, i, j)) * float(((i + j + 1) % 2) * 2 - 1);
			}
		}

		return Ct / Mat_Det(mat);
	}
};