#pragma once
#include "ext_vec2d.h"

namespace ext
{
	struct Scalar
	{
		static Scalar Make(ext::vec2d<float> m, ext::vec2d<float> c) { return { m,c }; }
		//as in f(x) = mx + c
		ext::vec2d<float> m = { 0.0f,0.0f };
		//as in f(x) = mx + c
		ext::vec2d<float> c = { 0.0f,0.0f };
	};
};