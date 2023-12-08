#include "ext_win32.h"
#include <windowsx.h>

using namespace ext;

void ext::ShowCursorX(bool bShow)
{
	for (int s = ShowCursor(bShow); s != (int)bShow - 1; s = ShowCursor(s < 0));
}


WinMenu::WinMenu()
{
	hMenu = CreateMenu();
}

WinMenu& WinMenu::AddButton(std::wstring label, int id, int state)
{
	MENUITEMINFOW mii = { 0 };
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE;
	mii.fState = state;
	mii.wID = id;
	mii.cch = label.size();
	mii.dwTypeData = new wchar_t[label.size() + 1];
	for (unsigned i = 0; i < label.size(); i++)
		mii.dwTypeData[i] = label[i];
	mii.dwTypeData[label.size()] = 0;
	InsertMenuItemW(hMenu, -1, TRUE, &mii);
	miis[id] = mii;
	return *this;
}

WinMenu& WinMenu::AddSubMenu(std::wstring label, const WinMenu& SubMenu)
{
	MENUITEMINFOW mii = { 0 };
	mii.cbSize = sizeof(mii);
	mii.fMask = MIIM_SUBMENU | MIIM_STRING;
	mii.hSubMenu = SubMenu.hMenu;
	mii.cch = label.size();
	mii.dwTypeData = new wchar_t[label.size() + 1];
	for (size_t i = 0; i < label.size(); i++)
		mii.dwTypeData[i] = label[i];
	mii.dwTypeData[label.size()] = 0;
	InsertMenuItemW(hMenu, -1, TRUE, &mii);
	miis.insert(SubMenu.miis.begin(), SubMenu.miis.end());
	return *this;
}

WinMenu& WinMenu::AddSeparator()
{
	return *this;
}

MENUITEMINFOW* WinMenu::operator[](int id)
{
	if (miis.count(id))
	{
		return &miis[id];
	}
	return nullptr;
}

WinMenu::operator HMENU()
{
	return hMenu;
}


HCURSOR hCursorArrow = LoadCursor(NULL, IDC_ARROW);


Window::DefClass Window::DefClass::singleton;

Window::Window(const wchar_t* title, vec2d<int> cdim, const wchar_t* wnd_class, int window_style, int window_ex_style, HWND hParent)
{
	this->cdim = cdim;
	this->window_style = window_style;
	this->window_ex_style = window_ex_style;

	RECT rect = { 0 };
	rect.left = 20;
	rect.top = 40;
	rect.right = cdim.x + rect.left + ((window_style & WS_VSCROLL) ? GetSystemMetrics(SM_CXVSCROLL) : 0);
	rect.bottom = cdim.y + rect.top + ((window_style & WS_HSCROLL) ? GetSystemMetrics(SM_CYHSCROLL) : 0);
	AdjustWindowRect(&rect, window_style, FALSE);

	if (!wnd_class)
		wnd_class = DefClass::Get().lpszClassName;

	hWnd = CreateWindowExW(
		window_ex_style,
		wnd_class,
		title,
		window_style,
		rect.left, rect.top, rect.right - rect.left, rect.bottom - rect.top,
		hParent, NULL, GetModuleHandle(NULL), this
	);
}

HCURSOR Window::DefCursor() const
{
	return hCursorArrow;
}

LRESULT CALLBACK Window::WndProcSetup(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (msg == WM_CREATE)
	{
		CREATESTRUCTW* cs = (CREATESTRUCTW*)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)cs->lpCreateParams);
		SetWindowLongPtr(hWnd, GWLP_WNDPROC, (LONG_PTR)WndProcAdaptor);
	}
	return DefWindowProcW(hWnd, msg, wParam, lParam);
}

LRESULT CALLBACK Window::WndProcAdaptor(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return ((Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA))->WndProc(hWnd, msg, wParam, lParam);
}

LRESULT Window::WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_MOUSEMOVE:
		mpos.x = GET_X_LPARAM(lParam);
		mpos.y = GET_Y_LPARAM(lParam);
		bMouseIn = mpos.x >= 0 && mpos.y >= 0 && mpos.x < cdim.x && mpos.y < cdim.y;
		if (!bTMESet)
		{
			bTMESet = true;
			SetCursor(hCursorArrow);
			TRACKMOUSEEVENT tme = { 0 };
			tme.cbSize = sizeof(tme);
			tme.dwFlags = TME_LEAVE;
			tme.hwndTrack = hWnd;
			TrackMouseEvent(&tme);
		}
		break;
	case WM_MOUSELEAVE:
		bTMESet = false;
		bMouseIn = false;
		break;
	case WM_SIZE:
		cdim.x = LOWORD(lParam);
		cdim.y = HIWORD(lParam);
		break;
	}
	return AppProc(hWnd, msg, wParam, lParam);
}