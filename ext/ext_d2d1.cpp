#include "ext_d2d1.h"
#pragma comment (lib, "d2d1.lib")
#pragma comment (lib, "dwrite.lib")
#pragma comment (lib, "Windowscodecs.lib")

using namespace ext;

CComPtr<ID2D1Factory> pFactory;
CComPtr<IDWriteFactory> pDWFactory;
CComPtr<IWICImagingFactory> pWicFactory;

int ext::MakeRGB(unsigned char r, unsigned char g, unsigned char b)
{
	return (r << 16) | (g << 8) | b;
}

CComPtr<ID2D1Factory> ext::d2dFactory()
{
	if (!pFactory)
	{
		D2D1CreateFactory(D2D1_FACTORY_TYPE_MULTI_THREADED, &pFactory);
	}
	return pFactory;
}
CComPtr<IDWriteFactory> ext::dwFactory()
{
	if (pDWFactory == nullptr)
	{
		DWriteCreateFactory(DWRITE_FACTORY_TYPE_ISOLATED, __uuidof(IDWriteFactory), (IUnknown**)&pDWFactory);
	}
	return pDWFactory;
}
CComPtr<IWICImagingFactory> ext::wicFactory()
{
	if (pWicFactory == nullptr)
	{
		CoInitialize(0);
		HRESULT hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_PPV_ARGS(&pWicFactory)
		);
	}
	return pWicFactory;
}

D2DGraphics::D2DGraphics(HWND hWnd)
{
	RECT rc;
	GetClientRect(hWnd, &rc);
	CComPtr<ID2D1HwndRenderTarget> pHwndTarget;
	d2dFactory()->CreateHwndRenderTarget(
		D2D1::RenderTargetProperties(),
		D2D1::HwndRenderTargetProperties(
			hWnd,
			D2D1::SizeU(rc.right, rc.bottom)),
		&pHwndTarget
	);
	pRenderTarget = pHwndTarget;
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &pSolidBrush);
}
D2DGraphics::D2DGraphics(CComPtr<ID2D1RenderTarget> pRenderTarget)
	:pRenderTarget(pRenderTarget)
{
	pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0), &pSolidBrush);
}
CComPtr<ID2D1Bitmap> D2DGraphics::CreateBitmap(const std::wstring& file_path)
{
	CComPtr<IWICBitmapDecoder> pDecoder;
	CComPtr<IWICBitmapFrameDecode> pSource;
	CComPtr<IWICFormatConverter> pConverter;
	CComPtr<ID2D1Bitmap> pBitmap;

	wicFactory()->CreateDecoderFromFilename(
		file_path.c_str(),
		NULL,
		GENERIC_READ,
		WICDecodeMetadataCacheOnLoad,
		&pDecoder
	);
	
	pDecoder->GetFrame(0, &pSource);

	wicFactory()->CreateFormatConverter(&pConverter);

	pConverter->Initialize(
		pSource.p,
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		NULL,
		0.f,
		WICBitmapPaletteTypeMedianCut
	);

	pRenderTarget->CreateBitmapFromWicBitmap(
		pConverter.p,
		NULL,
		&pBitmap
	);

	return pBitmap;
}
CComPtr<ID2D1Bitmap> ext::D2DGraphics::CreateBitmap(const Surface& surface)
{
	CComPtr<ID2D1Bitmap> pBitmap;
	HRESULT hr = pRenderTarget->CreateBitmap(
		D2D1::SizeU(surface.GetSize().x, surface.GetSize().y),
		surface.GetBuffer().get(),
		surface.GetSize().x * 4,
		D2D1::BitmapProperties(
			D2D1::PixelFormat(
				DXGI_FORMAT_B8G8R8A8_UNORM,
				D2D1_ALPHA_MODE_PREMULTIPLIED)),
		&pBitmap
	);
	return pBitmap;
}

TextFormat::TextFormat(const std::wstring& font, float size, DWRITE_WORD_WRAPPING wrap, DWRITE_FONT_WEIGHT weight, DWRITE_FONT_STYLE style, DWRITE_FONT_STRETCH stretch)
{
	dwFactory()->CreateTextFormat(
		font.c_str(), NULL,
		weight, style, stretch,
		size, L"pt-br", &pFormat
	);
	pFormat->SetWordWrapping(wrap);
}
CComPtr<IDWriteTextLayout> TextFormat::operator()(const std::wstring& string, float max_width, float max_height) const
{
	CComPtr<IDWriteTextLayout> layout;
	auto ww = pFormat->GetWordWrapping();
	if (max_width == 0.0f)
	{
		pFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);
	}
	dwFactory()->CreateTextLayout(
		string.c_str(), string.size(),
		pFormat, max_width, max_height,
		&layout
	);
	pFormat->SetWordWrapping(ww);
	return layout;
}
IDWriteTextFormat* TextFormat::operator->()
{
	return pFormat.p;
}
TextFormat::operator IDWriteTextFormat*()
{
	return pFormat.p;
}