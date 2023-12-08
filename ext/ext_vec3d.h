#pragma once
#include <string>
#include <cmath>
#include "ext_vec2d.h"

namespace ext
{
	template <typename T>
	struct vec3d : public vec2d<T>
	{
		T z;

		template <typename J>
		T dot(const vec3d<J>& B) const
		{
			return this->x * B.x + this->y * B.y + z * B.z;
		}
		template <typename J>
		vec3d cross(const vec3d<J>& B) const
		{
			return {
				this->y * B.z - z * B.y,
				-(this->x * B.z - z * B.x),
				this->x * B.y - this->y * B.x,
			};
		}

		vec3d operator-() const
		{
			return { -this->x, -this->y, -z };
		}
		template <typename J>
		vec3d<J> to() const
		{
			return { (J)this->x,(J)this->y,(J)z };
		}

		std::wstring wprint() const
		{
			return std::to_wstring(this->x) + L", " + std::to_wstring(this->y) + L", " + std::to_wstring(z);
		}
		std::string print() const
		{
			return std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(z);
		}

		vec3d add_x(T x_) const
		{
			return { this->x + x_, this->y, z };
		}
		vec3d add_y(T y_) const
		{
			return { this->x, this->y + y_, z };
		}
		vec3d add_z(T z_) const
		{
			return { this->x, this->y, z + z_ };
		}
		vec3d mult_x(T x_) const
		{
			return { this->x * x_, this->y, z };
		}
		vec3d mult_y(T y_) const
		{
			return { this->x, this->y * y_, z };
		}
		vec3d mult_z(T z_) const
		{
			return { this->x, this->y, z * z_ };
		}

		vec3d norm() const
		{
			return *this / mod();
		}

		T mod() const
		{
			return std::sqrt(this->x * this->x + this->y * this->y + z * z);
		}
		T vol() const
		{
			return this->x * this->y * z;
		}

		template <typename J>
		bool operator==(const vec3d<J>& rhs) const
		{
			return this->x == rhs.x && this->y == rhs.y && z == rhs.z;
		}
		template <typename J>
		bool operator!=(const vec3d<J>& rhs) const
		{
			return !((*this) == rhs);
		}

		template <typename J>
		vec3d operator+(const vec3d<J>& B) const
		{
			return { this->x + (T)B.x, this->y + (T)B.y, z + (T)B.z };
		}
		template <typename J>
		vec3d operator-(const vec3d<J>& B) const
		{
			return { this->x - (T)B.x, this->y - (T)B.y, z - (T)B.z };
		}
		template <typename J>
		vec3d operator*(const vec3d<J>& B) const
		{
			return { this->x * (T)B.x, this->y * (T)B.y, z * (T)B.z };
		}
		template <typename J>
		vec3d operator/(const vec3d<J>& B) const
		{
			return { this->x / (T)B.x, this->y / (T)B.y, z / (T)B.z };
		}

		template <typename J>
		vec3d& operator=(const vec3d<J>& v)
		{
			this->x = (T)v.x;
			this->y = (T)v.y;
			z = (T)v.z;
			return *this;
		}
		template <typename J>
		vec3d& operator+=(const vec3d<J>& v)
		{
			this->x += (T)v.x;
			this->y += (T)v.y;
			z += (T)v.z;
			return *this;
		}
		template <typename J>
		vec3d& operator-=(const vec3d<J>& v)
		{
			this->x -= (T)v.x;
			this->y -= (T)v.y;
			z -= (T)v.z;
			return *this;
		}
		template <typename J>
		vec3d& operator*=(const vec3d<J>& v)
		{
			this->x *= (T)v.x;
			this->y *= (T)v.y;
			z *= (T)v.z;
			return *this;
		}
		template <typename J>
		vec3d& operator/=(const vec3d<J>& v)
		{
			this->x /= (T)v.x;
			this->y /= (T)v.y;
			z /= (T)v.z;
			return *this;
		}

		template <typename J>
		vec3d operator+(const J& B) const
		{
			return { this->x + (T)B, this->y + (T)B, z + (T)B };
		}
		template <typename J>
		vec3d operator-(const J& B) const
		{
			return { this->x - (T)B, this->y - (T)B, z - (T)B };
		}
		template <typename J>
		vec3d operator*(const J& B) const
		{
			return { this->x * (T)B, this->y * (T)B, z * (T)B };
		}
		template <typename J>
		vec3d operator/(const J& B) const
		{
			return { this->x / (T)B, this->y / (T)B, z / (T)B };
		}

		template <typename J>
		vec3d& operator=(const J& v)
		{
			this->x = (T)v;
			this->y = (T)v;
			z = (T)v;
			return *this;
		}
		template <typename J>
		vec3d& operator+=(const J& v)
		{
			this->x += (T)v;
			this->y += (T)v;
			z += (T)v;
			return *this;
		}
		template <typename J>
		vec3d& operator-=(const J& v)
		{
			this->x -= (T)v;
			this->y -= (T)v;
			z -= (T)v;
			return *this;
		}
		template <typename J>
		vec3d& operator*=(const J& v)
		{
			this->x *= (T)v;
			this->y *= (T)v;
			z *= (T)v;
			return *this;
		}
		template <typename J>
		vec3d& operator/=(const J& v)
		{
			this->x /= (T)v;
			this->y /= (T)v;
			z /= (T)v;
			return *this;
		}
	};

	template <typename T, typename J>
	vec3d<J> operator+(const J& lhs, const vec3d<T>& rhs)
	{
		return { lhs + (J)rhs.x,lhs + (J)rhs.y,lhs + (J)rhs.z };
	}
	template <typename T, typename J>
	vec3d<J> operator-(const J& lhs, const vec3d<T>& rhs)
	{
		return { lhs - (J)rhs.x,lhs - (J)rhs.y,lhs - (J)rhs.z };
	}
	template <typename T, typename J>
	vec3d<J> operator*(const J& lhs, const vec3d<T>& rhs)
	{
		return { lhs * (J)rhs.x,lhs * (J)rhs.y,lhs * (J)rhs.z };
	}
	template <typename T, typename J>
	vec3d<J> operator/(const J& lhs, const vec3d<T>& rhs)
	{
		return { lhs / (J)rhs.x,lhs / (J)rhs.y,lhs / (J)rhs.z };
	}
};