#include "ext_canvas.h"
#include "ext_d2d1.h"
#pragma comment (lib, "Windowscodecs.lib")

namespace ext
{
	Surface::Surface(ext::vec2d<int> size)
		:size(size)
	{
		buffer.reset(new Color[size.x * size.y]);
	}
	Surface::Surface(const wchar_t* file_path)
	{
		HRESULT hr;

		CComPtr<IWICBitmapDecoder> pDecoder;
		hr = ext::wicFactory()->CreateDecoderFromFilename(
			file_path,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&pDecoder
		);

		CComPtr<IWICBitmapFrameDecode> pSource;
		hr = pDecoder->GetFrame(0, &pSource);

		CComPtr<IWICFormatConverter> pConverter;
		ext::wicFactory()->CreateFormatConverter(&pConverter);
		pConverter->Initialize(
			pSource.p,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.f,
			WICBitmapPaletteTypeMedianCut
		);

		CComPtr<IWICBitmap> pBitmap;
		hr = ext::wicFactory()->CreateBitmapFromSource(pConverter.p, WICBitmapCacheOnDemand, &pBitmap);

		CComPtr<IWICBitmapLock> pBitmapLock;
		UINT w, h;
		hr = pBitmap->GetSize(&w, &h);
		WICRect rcLock = { 0,0,(INT)w,(INT)h };
		hr = pBitmap->Lock(&rcLock, WICBitmapLockWrite, &pBitmapLock);

		UINT buffer_size;
		BYTE* input;

		hr = pBitmapLock->GetDataPointer(&buffer_size, &input);

		size.x = w;
		size.y = h;
		buffer.reset(new Color[buffer_size]);
		for (size_t i = 0; i < buffer_size / 4; i++)
			buffer.get()[i] = { input[i * 4], input[i * 4 + 1], input[i * 4 + 2], input[i * 4 + 3] };
	}
	void Surface::Shares(const Surface& source)
	{
		size = source.size;
		buffer = source.buffer;
	}
	void Surface::Copies(const Surface& source)
	{
		if (size != source.size || buffer.get() == source.buffer.get())
		{
			size = source.size;
			buffer.reset(new Color[size.x * size.y]);
		}
		for (int i = 0; i < size.x * size.y; i++)
			buffer.get()[i] = source.buffer.get()[i];
	}
	void Surface::Resize(ext::vec2d<int> new_size)
	{
		Color* new_buffer = new Color[new_size.x * new_size.y];
		if (buffer)
		{
			for (int y = std::min(new_size.y, size.y) - 1; y >= 0; y--)
			{
				for (int x = std::min(new_size.x, size.x) - 1; x >= 0; x--)
				{
					new_buffer[x + y * new_size.x] = buffer.get()[x + y * size.x];
				}
			}
		}
		this->size = size;
		buffer.reset(new_buffer);
	}
	const Color& Surface::operator[](const vec2d<int>& p) const 
	{ 
		if (p.x < 0 || p.x >= size.x || p.y < 0 || p.y >= size.y)
			return { 255,0,255 };
		return buffer.get()[p.x + p.y * size.x];
	}

	Canvas::Canvas(ext::vec2d<int> size)
		:Surface(size)
	{
	}
	void Canvas::PutPixel(ext::vec2d<int> p, Color color)
	{
		if (p.x >= 0 && p.x < size.x && p.y >= 0 && p.y < size.y)
		{
			buffer.get()[p.x + p.y * size.x] = color;
		}
	}
	void Canvas::PutPixelNoCheck(ext::vec2d<int> p, Color color)
	{
		buffer.get()[p.x + p.y * size.x] = color;
	}
	void Canvas::Clear(Color color)
	{
		for (int i = 0; i < size.x * size.y; i++)
			buffer.get()[i] = color;
	}
	void Canvas::DrawTriangle(ext::vec2d<float> a, ext::vec2d<float> b, ext::vec2d<float> c, Color color)
	{
		ext::vec2d<float> d;
		//make intemediate vertex ('d') to split the triangle
		{
			//sort verticies based on height
			if (a.y > b.y)
				std::swap(a, b);
			if (a.y > c.y)
				std::swap(a, c);
			if (b.y > c.y)
				std::swap(c, b);

			//'d' is in between 'a' and 'c' at the height of 'b'
			float k = (b.y - a.y) / (c.y - a.y);
			//linear interpolation
			d = (c - a) * k + a;
			//d.tx = (c.tx - a.tx) * k + a.tx;
		}

		if (b.x > d.x)
			std::swap(b, d);

		//draw flat bottom triangle (abd)
		{
			int y = (int)std::round(a.y);
			const float dx0 = (b.x - a.x) / (b.y - a.y);
			const float dx1 = (d.x - a.x) / (b.y - a.y);
			//x0 and x1 at y - apy + 0.5
			float x0 = a.x + dx0 * (0.5f - (a.y - (float)y));
			float x1 = a.x + dx1 * (0.5f - (a.y - (float)y));
			for (int end_y = (int)std::round(b.y); y < end_y; y++)
			{
				for (int x = (int)std::round(x0), end_x = (int)std::round(x1); x < end_x; x++)
				{
					PutPixel({ x, y }, color);
				}
				x0 += dx0;
				x1 += dx1;
			}
		}

		//draw flat top triangle (bdc)
		{
			int y = (int)std::round(b.y);
			const float dx0 = (c.x - b.x) / (c.y - b.y);
			const float dx1 = (c.x - d.x) / (c.y - b.y);
			//x0 and x1 at y - apy + 0.5
			float x0 = b.x + dx0 * (0.5f - (b.y - (float)y));
			float x1 = d.x + dx1 * (0.5f - (d.y - (float)y));
			for (int end_y = (int)std::round(c.y); y < end_y; y++)
			{
				for (int x = (int)std::round(x0), end_x = (int)std::round(x1); x < end_x; x++)
				{
					PutPixel({ x, y }, color);
				}
				x0 += dx0;
				x1 += dx1;
			}
		}
	}
}