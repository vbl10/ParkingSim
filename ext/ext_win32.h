#pragma once
#define NOMINMAX
#include <Windows.h>
#include "ext_vec2d.h"
#include <unordered_map>

namespace ext
{
	void ShowCursorX(bool bShow = true);

	class WinMenu
	{
	public:
		WinMenu();

		WinMenu& AddButton(std::wstring label, int id, int state = 0);
		WinMenu& AddSubMenu(std::wstring label, const WinMenu& SubMenu);
		WinMenu& AddSeparator();

		MENUITEMINFOW* operator[](int id);
		operator HMENU();

		HMENU hMenu;
	private:
		std::unordered_map<int, MENUITEMINFOW> miis;
	};

	class Window
	{
	public:
		class DefClass
		{
			static DefClass singleton;
			WNDCLASSEXW wc;
			DefClass()
			{
				wc.cbSize = sizeof(wc);
				wc.lpfnWndProc = WndProcSetup;
				wc.hInstance = GetModuleHandle(NULL);
				wc.lpszClassName = L"default";
				wc.cbWndExtra = sizeof(Window);

				RegisterClassExW(&wc);
			}
			~DefClass()
			{
				UnregisterClassW(wc.lpszClassName, wc.hInstance);
			}
		public:
			static const WNDCLASSEXW& Get() { return singleton.wc; }
		};

	protected:
		Window(const wchar_t* title, vec2d<int> cdim, const wchar_t* wnd_class = nullptr, int window_style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, int window_ex_style = 0, HWND hParent = NULL);
		Window(const Window&) = delete;

		virtual ~Window() = default;
	public:
		HCURSOR DefCursor() const;

		int window_style, window_ex_style;
		vec2d<int> cdim;
		HWND hWnd;
		vec2d<int> mpos = { 0 };
		bool bMouseIn = false;

	private:
		bool bTMESet = false;

		static LRESULT CALLBACK WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		static LRESULT CALLBACK WndProcAdaptor(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		LRESULT WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
		virtual LRESULT AppProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
	};
};