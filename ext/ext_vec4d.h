#pragma once
#include "ext_vec3d.h"

namespace ext
{
	template <typename T>
	struct vec4d : public vec3d<T>
	{
		T w;
	};
};