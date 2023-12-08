#pragma once
#define NOMINMAX
#include <d2d1.h>
#include <dwrite.h>
#include <atlbase.h>
#include <wincodec.h>
#include <string>
#include "ext_canvas.h"

namespace ext
{
	int MakeRGB(unsigned char r, unsigned char g, unsigned char b);

	CComPtr<ID2D1Factory> d2dFactory();
	CComPtr<IDWriteFactory> dwFactory();
	CComPtr<IWICImagingFactory> wicFactory();

	struct TextFormat
	{
		TextFormat(const std::wstring& font, float size, DWRITE_WORD_WRAPPING wrap = DWRITE_WORD_WRAPPING_NO_WRAP, DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);
		CComPtr<IDWriteTextLayout> operator()(const std::wstring& string, float max_width = 0.0f, float max_height = 0.0f) const;
		IDWriteTextFormat* operator->();
		operator IDWriteTextFormat*();
		CComPtr<IDWriteTextFormat> pFormat;
	};

	class D2DGraphics
	{
	public:
		D2DGraphics(HWND hWnd);
		D2DGraphics(CComPtr<ID2D1RenderTarget> pRenderTarget);
		CComPtr<ID2D1Bitmap> CreateBitmap(const std::wstring& file_path);
		CComPtr<ID2D1Bitmap> CreateBitmap(const Surface& surface);

		CComPtr<ID2D1RenderTarget> pRenderTarget;
		CComPtr<ID2D1SolidColorBrush> pSolidBrush;
	};
}

