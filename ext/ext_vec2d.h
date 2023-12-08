#pragma once
#include <string>
#include <cmath>
#define NOMINMAX
#include <Windows.h>
#include <d2d1.h>

namespace ext
{
	template <typename T>
	struct vec2d
	{
		T x, y;

		vec2d operator-() const
		{
			return { -x, -y };
		}
		template <typename J>
		vec2d<J> to() const
		{
			return { (J)x,(J)y };
		}
		
		operator D2D1_SIZE_F() const
		{
			return D2D1::SizeF(x, y);
		}
		operator D2D1_POINT_2F() const
		{
			return D2D1::Point2F(x, y);
		}
		operator D2D1_POINT_2U() const
		{
			return D2D1::Point2U(x, y);
		}
		operator POINT() const
		{
			return { (LONG)x,(LONG)y };
		}

		std::wstring printw() const
		{
			return std::to_wstring(x) + L", " + std::to_wstring(y);
		}
		std::string print() const
		{
			return std::to_string(x) + L", " + std::to_string(y);
		}

		vec2d add_x(T x_) const
		{
			return { x + x_, y };
		}
		vec2d add_y(T y_) const
		{
			return { x, y + y_ };
		}
		vec2d mult_x(T x_) const
		{
			return { x * x_, y };
		}
		vec2d mult_y(T y_) const
		{
			return { x, y * y_ };
		}

		vec2d norm() const
		{
			return *this / mod();
		}

		T mod() const
		{
			return std::sqrt(x * x + y * y);
		}
		T area() const
		{
			return x * y;
		}

		template <typename J>
		vec2d operator+(const vec2d<J>& B) const
		{
			return { x + (T)B.x, y + (T)B.y };
		}
		template <typename J>
		vec2d operator-(const vec2d<J>& B) const
		{
			return { x - (T)B.x, y - (T)B.y };
		}
		template <typename J>
		vec2d operator*(const vec2d<J>& B) const
		{
			return { x * (T)B.x, y * (T)B.y };
		}
		template <typename J>
		vec2d operator/(const vec2d<J>& B) const
		{
			return { x / (T)B.x, y / (T)B.y };
		}

		template <typename J>
		vec2d& operator=(const vec2d<J>& v)
		{
			x = (T)v.x;
			y = (T)v.y;
			return *this;
		}
		template <typename J>
		vec2d& operator+=(const vec2d<J>& v)
		{
			x += (T)v.x;
			y += (T)v.y;
			return *this;
		}
		template <typename J>
		vec2d& operator-=(const vec2d<J>& v)
		{
			x -= (T)v.x;
			y -= (T)v.y;
			return *this;
		}
		template <typename J>
		vec2d& operator*=(const vec2d<J>& v)
		{
			x *= (T)v.x;
			y *= (T)v.y;
			return *this;
		}
		template <typename J>
		vec2d& operator/=(const vec2d<J>& v)
		{
			x /= (T)v.x;
			y /= (T)v.y;
			return *this;
		}

		template <typename J>
		vec2d operator+(const J& B) const
		{
			return { x + (T)B, y + (T)B };
		}
		template <typename J>
		vec2d operator-(const J& B) const
		{
			return { x - (T)B, y - (T)B };
		}
		template <typename J>
		vec2d operator*(const J& B) const
		{
			return { x * (T)B, y * (T)B };
		}
		template <typename J>
		vec2d operator/(const J& B) const
		{
			return { x / (T)B, y / (T)B };
		}

		template <typename J>
		vec2d& operator=(const J& v)
		{
			x = (T)v;
			y = (T)v;
			return *this;
		}
		template <typename J>
		vec2d& operator+=(const J& v)
		{
			x += (T)v;
			y += (T)v;
			return *this;
		}
		template <typename J>
		vec2d& operator-=(const J& v)
		{
			x -= (T)v;
			y -= (T)v;
			return *this;
		}
		template <typename J>
		vec2d& operator*=(const J& v)
		{
			x *= (T)v;
			y *= (T)v;
			return *this;
		}
		template <typename J>
		vec2d& operator/=(const J& v)
		{
			x /= (T)v;
			y /= (T)v;
			return *this;
		}

		//boolean operations
		template <class J>
		bool operator == (const vec2d<J>& rhs) const
		{
			return x == (T)rhs.x && y == (T)rhs.y;
		}
		template <class J>
		bool operator != (const vec2d<J>& rhs) const
		{
			return x != (T)rhs.x || y != (T)rhs.y;
		}

		template <class J>
		bool operator == (J rhs)const
		{
			return x == (T)rhs && y == (T)rhs;
		}
		template <class J>
		bool operator != (J rhs)const
		{
			return x != (T)rhs || y != (T)rhs;
		}
	};

	template <typename T, typename J>
	vec2d<J> operator+(const J& lhs, const vec2d<T>& rhs)
	{
		return { lhs + (J)rhs.x,lhs + (J)rhs.y };
	}
	template <typename T, typename J>
	vec2d<J> operator-(const J& lhs, const vec2d<T>& rhs)
	{
		return { lhs - (J)rhs.x,lhs - (J)rhs.y };
	}
	template <typename T, typename J>
	vec2d<J> operator*(const J& lhs, const vec2d<T>& rhs)
	{
		return { lhs * (J)rhs.x,lhs * (J)rhs.y };
	}
	template <typename T, typename J>
	vec2d<J> operator/(const J& lhs, const vec2d<T>& rhs)
	{
		return { lhs / (J)rhs.x,lhs / (J)rhs.y };
	}

	
};