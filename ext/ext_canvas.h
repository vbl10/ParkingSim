#pragma once
#include "ext_vec2d.h"
#include <memory>

namespace ext
{
	struct Color
	{
		unsigned char b, g, r, a = 255;
	};

	struct Surface
	{
		Surface() = default;
		Surface(const wchar_t* file_path);
		Surface(ext::vec2d<int> size);

		void Shares(const Surface& source);
		void Copies(const Surface& source);
		void Resize(ext::vec2d<int> new_size);

		const ext::vec2d<int> GetSize() const { return size; }

		std::shared_ptr<Color[]> GetBuffer() const { return buffer; }
		const Color& operator[](const vec2d<int>& p) const;

	protected:
		ext::vec2d<int> size = { 0,0 };
		std::shared_ptr<Color[]> buffer = nullptr;
	};

	class Canvas : public Surface
	{
	public:
		Canvas(ext::vec2d<int> size);
		Canvas(const Canvas&) = delete;
		Canvas& operator=(const Canvas&) = delete;

		void PutPixel(ext::vec2d<int> p, Color color);
		void PutPixelNoCheck(ext::vec2d<int> p, Color color);
		void DrawTriangle(ext::vec2d<float> a, ext::vec2d<float> b, ext::vec2d<float> c, Color color);
		virtual void Clear(Color color = { 0,0,0 });
	};
};