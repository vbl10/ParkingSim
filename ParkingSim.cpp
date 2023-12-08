#include "ext_win32.h"
#include "ext_d2d1.h"
#include <chrono>
#include <vector>

using namespace ext;

class App : public Window, public D2DGraphics
{
public:
	App()
		:Window(L"ParkingSim", { 600,500 }), D2DGraphics(hWnd)
	{
		ShowWindow(hWnd, SW_SHOW);
	}
private:
	LRESULT AppProc(HWND, UINT msg, WPARAM wParam, LPARAM lParam) override
	{
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
			break;
		}
		return DefWindowProcW(hWnd, msg, wParam, lParam);
	}
};

struct Car
{
	void Draw(D2DGraphics& gfx, const float& fScale, const D2D1_MATRIX_3X2_F& mat00, bool bHighlight) const
	{
		gfx.pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(-fAngle * 180.0f / 3.14159f, D2D1::Point2F(pos.x * fScale, -pos.y * fScale)) * mat00);
		gfx.pSolidBrush->SetColor(D2D1::ColorF(bHighlight ? D2D1::ColorF::White : D2D1::ColorF::Gray));
		gfx.pRenderTarget->DrawRectangle(
			D2D1::RectF(
				(pos.x + body_model[0].x) * fScale,
				-(pos.y + body_model[0].y) * fScale,
				(pos.x + body_model[2].x) * fScale,
				-(pos.y + body_model[2].y) * fScale
			), gfx.pSolidBrush, 2.0f
		);
		gfx.pRenderTarget->DrawLine(
			D2D1::Point2F(pos.x * fScale, -(pos.y + body_model[0].y) * fScale),
			D2D1::Point2F(pos.x * fScale, -(pos.y - body_model[0].y) * fScale),
			gfx.pSolidBrush, 2.0f);

		if (bHighlight && abs(fSteerAm) > 0.0005f && bDrawTurningCircle)
		{
			gfx.pRenderTarget->DrawEllipse(
				D2D1::Ellipse(
					D2D1::Point2F(
						fScale * pos.x,
						fScale * -(pos.y + fTurningRadius)
					),
					fScale * fTurningRadius, fScale * fTurningRadius
				), gfx.pSolidBrush
			);
		}
		gfx.pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
	}
	//value ranges from -1 to 1
	void Steer(float fSteerAm_)
	{
		fSteerAm = std::min(1.0f, std::max(-1.0f, fSteerAm_));
		fTurningRadius = fMinTurningRadius / fSteerAm;
	}
	const float& GetSteer() const
	{
		return fSteerAm;
	}
	void Move(float fSpeed, const float& fElapsedTime)
	{
		if (fSteerAm != 0.0f)
		{
			auto d = fTurningRadius * vec2d<float>(sin(fAngle), -cos(fAngle)), c = pos - d;
			float fTheta = fSpeed * fElapsedTime / fTurningRadius;
			fAngle += fTheta;
			pos.x = cos(fTheta) * d.x - sin(fTheta) * d.y + c.x;
			pos.y = cos(fTheta) * d.y + sin(fTheta) * d.x + c.y;
		}
		else
		{
			pos.x += fSpeed * fElapsedTime * cos(fAngle);
			pos.y += fSpeed * fElapsedTime * sin(fAngle);
		}
	}
	
	//vw fox as reference
	vec2d<float> size = { 3.828f,1.66f };
	static bool bDrawTurningCircle;
	float fAngle = 0.0f;
	vec2d<float> pos;
private:
	/* calculation of MinTurningRadius
	* R = wall to wall turning radius
	* L = length bumper to bumper
	* W = width mirror to mirror
	* Y = distance from rear bumper to rear axle
	* x = distance from turning center to rear axle's center
	* x = sqrt(R² - (L - Y)²) - W/2
	*/
	static float fMinTurningRadius; //from turning center to the rear axle's center
	//origin at the center of the rear axle
	vec2d<float> body_model[4] = {
		{ 3.299f,-0.83f }, //front left
		{ 3.299f, 0.83f }, //front right
		{-0.529f, 0.83f }, //rear right
		{-0.529f,-0.83f }  //rear left
	};
	float fSteerAm = 0.0f, fTurningRadius = 0.0f;
};
bool Car::bDrawTurningCircle = true;
float Car::fMinTurningRadius = 3.57f;

int CALLBACK wWinMain(HINSTANCE, HINSTANCE, LPWSTR, int)
{
	App app;
	auto mat00 = D2D1::Matrix3x2F::Translation(app.cdim.x / 2, app.cdim.y / 2);

	CComPtr<IDWriteTextFormat> pTextFormat;
	dwFactory()->CreateTextFormat(
		L"Azaret Mono", nullptr,
		DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		16.0f, L"pt-br", &pTextFormat
	);
	pTextFormat->SetWordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP);

	MSG msg;
	float fElapsedTime = 0.0f;
	struct Key
	{
		bool bPressed = false, bHeld = false, bReleased = false;
	};
	Key kbd[0xff];

	float curb_y = -2.0f;
	std::vector<Car> cars(3);
	for (int i = 0; i < 3; i++)
	{
		cars[i].pos.x = -6.0f + cars[i].size.x * 1.25f * i;
		cars[i].pos.y = curb_y + 0.3f + cars[i].size.y * 0.5f;
	}
	int nActiveCar = 1;
	
	float fScale = 40.0f; // 1 meter = 40 pixels

	while (1)
	{
		auto tp1 = std::chrono::steady_clock::now();
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessageW(&msg);
			if (msg.message == WM_QUIT)
			{
				break;
			}
		}
		for (int i = 0; i < 0xff; i++)
		{
			if (GetAsyncKeyState(i))
			{
				if (kbd[i].bHeld)
				{
					kbd[i].bPressed = false;
				}
				else
				{
					kbd[i].bPressed = true;
					kbd[i].bHeld = true;
				}
			}
			else if (kbd[i].bHeld)
			{
				kbd[i].bHeld = false;
				kbd[i].bReleased = true;
			}
			else
			{
				kbd[i].bReleased = false;
			}
		}

		if (kbd[VK_UP].bHeld || kbd[VK_DOWN].bHeld)
		{
			cars[nActiveCar].Move((kbd[VK_SHIFT].bHeld ? 2.0f : 7.0f) * (kbd[VK_DOWN].bHeld ? -1.0f : 1.0f), fElapsedTime);
		}
		if (kbd[VK_RIGHT].bHeld || kbd[VK_LEFT].bHeld)
		{
			cars[nActiveCar].Steer(cars[nActiveCar].GetSteer() + fElapsedTime * (kbd[VK_SHIFT].bHeld ? 0.1f : 1.0f) * (kbd[VK_RIGHT].bHeld ? -1.0f : 1.0f));
		}
		if (kbd[VK_SPACE].bHeld)
		{
			cars[nActiveCar].Steer(0.0f);
		}
		if (kbd[VK_TAB].bPressed)
		{
			nActiveCar = (nActiveCar + 1) % cars.size();
		}
		if (kbd['C'].bPressed)
		{
			Car::bDrawTurningCircle = !Car::bDrawTurningCircle;
		}
		if (kbd['S'].bPressed)
		{
			cars.push_back(cars[nActiveCar]);
		}
		if (kbd['A'].bPressed)
		{
			if (cars.size() > 1)
				cars.erase(cars.begin() + nActiveCar);
			nActiveCar %= cars.size();
		}
		

		app.pRenderTarget->BeginDraw();
		app.pRenderTarget->Clear();

		app.pRenderTarget->SetTransform(mat00);
		//curb
		app.pSolidBrush->SetColor(D2D1::ColorF(0xff0000));
		app.pRenderTarget->DrawLine(D2D1::Point2F(-app.cdim.x/2, -curb_y * fScale), D2D1::Point2F(app.cdim.x/2, -curb_y * fScale), app.pSolidBrush);
		
		//ideal distance
		app.pSolidBrush->SetColor(D2D1::ColorF(0xff00, 0.3f));
		app.pRenderTarget->FillRectangle(
			D2D1::RectF(-app.cdim.x/2,-(curb_y + 0.45f)*fScale,app.cdim.x/2,-(curb_y + 0.15f)*fScale),
			app.pSolidBrush
		);


		for (const auto& c : cars)
		{
			c.Draw(app, fScale, mat00, &c == &cars[nActiveCar]);
		}

		//draw steering wheel
		{
			static const float fRadius = 40.0f;
			static const float fHBarOffset = 10.0f;
			static const float fHBarLength = 2.0f * (sqrt(fRadius * fRadius - fHBarOffset * fHBarOffset));
			static const D2D1_ELLIPSE circle = D2D1::Ellipse(D2D1::Point2F(fRadius + 10.0f, fRadius + 10.0f), fRadius, fRadius);
			static const D2D1_POINT_2F
				a = D2D1::Point2F(circle.point.x - fHBarLength * 0.5f, circle.point.y - fHBarOffset),
				b = D2D1::Point2F(a.x + fHBarLength, a.y);
			app.pSolidBrush->SetColor(D2D1::ColorF(0xffffff));
			app.pRenderTarget->DrawEllipse(circle,app.pSolidBrush, 2.0f);
			app.pRenderTarget->SetTransform(D2D1::Matrix3x2F::Rotation(cars[nActiveCar].GetSteer() * -540.0f, circle.point));
			app.pRenderTarget->DrawLine(a, b, app.pSolidBrush, 2.0f);
			app.pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
		}

		app.pRenderTarget->EndDraw();
		fElapsedTime = std::chrono::duration<float>(std::chrono::steady_clock::now() - tp1).count();
	}

	return 0;
}