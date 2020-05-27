#include "WindowsModule.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <memory.h>
#include <wchar.h>
#include <math.h>
#include <sstream>
#include <fstream>
#include <iostream>

extern HWND hWnd;
static bool KeyBefore[5];
BYTE key_state[256];
BYTE prev_key_state[256];
int mouseX;
int mouseY;

namespace winmod {
	void app_init(WNDCLASS* wcex, HINSTANCE hInstance, float WindowWidth, float WindowHeight) {
		if (FAILED(CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE))) {
			MessageBox(hWnd, TEXT("COM_initialize_field"), TEXT("Error"), MB_ICONSTOP);
			exit(1);
		}
		win_init(wcex, hInstance, WindowWidth, WindowHeight);
	}

	void win_end() {
		CoUninitialize();
	}

	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {
		case WM_KEYDOWN:
			switch (wParam) {
			case VK_ESCAPE:
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				break;
			}
			break;
		case WM_MOUSEMOVE:
			mouseX = (LOWORD(lParam)) * (GetDpiForSystem() / 96.f);
			mouseY = (HIWORD(lParam)) * (GetDpiForSystem() / 96.f);
			break;
		case WM_CLOSE:
			PostMessage(hWnd, WM_DESTROY, 0, 0);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		return(DefWindowProc(hWnd, message, wParam, lParam));
	}

	int win_init(WNDCLASS* wcex, HINSTANCE hInstance, float WindowWidth, float WindowHeight) {
		try {
			for (int i = 0; i < 5; i++)
				KeyBefore[i] = true;
			setlocale(LC_ALL, "japanese");
			wcex->style = CS_HREDRAW | CS_VREDRAW;
			wcex->lpfnWndProc = WndProc;
			wcex->cbClsExtra = 0;
			wcex->cbWndExtra = 0;
			wcex->hInstance = hInstance;
			wcex->hIcon = NULL;
			wcex->hCursor = NULL;
			wcex->hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			wcex->lpszMenuName = NULL;
			wcex->lpszClassName = "WindowsOfWindow0001";
			RegisterClass(wcex);
			hWnd = CreateWindow("WindowsOfWindow0001",
				"ÉåÉAÇÃâ~ëÏéèÍ",
				WS_POPUP | WS_VISIBLE | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
				0,
				0,
				(unsigned int)(WindowWidth * (GetDpiForSystem() / 96.f)),
				(unsigned int)(WindowHeight * (GetDpiForSystem() / 96.f)),
				NULL,
				NULL,
				hInstance,
				NULL);
			set_window_size(WindowWidth, WindowHeight);
			return 0;
		}
		catch (char* ex) {
			return 1;
		}
	}

	bool set_window_size(int x, int y) {
		RECT rc;
		RECT wn;

		GetWindowRect(hWnd, &wn);
		GetClientRect(hWnd, &rc);
		float newWidth = (wn.right - wn.left) - (rc.right - rc.left) + x;
		float newHeight = (wn.bottom - wn.top) - (rc.bottom - rc.top) + y;

		return SetWindowPos(hWnd, NULL, 0, 0, newWidth * (GetDpiForSystem() / 96.f), newHeight * (GetDpiForSystem() / 96.f), SWP_NOMOVE);
	}

	bool set_window_pos(int x, int y) {
		return SetWindowPos(hWnd, NULL, x, y, 0, 0, SWP_NOSIZE);
	}

	bool set_title(LPCWSTR TitleText) {
		return SetWindowTextW(hWnd, TitleText);
	}

	void load_tabled_stringW(UINT StringID, LPWSTR* GetStr, int size) {
		WCHAR* str;
		str = new WCHAR[size];
		LoadStringW(GetModuleHandle(NULL), StringID, str, size);
		*GetStr = str;
	}

	void load_tabled_String(UINT StringID, LPSTR* GetStr, int size) {
		CHAR* str;
		str = new CHAR[size];
		LoadString(GetModuleHandle(NULL), StringID, str, size);
		*GetStr = str;
	}

	void set_rand_seed() {
		srand((unsigned int)time(NULL));
		for (int i = 0; i < 10; i++)
			rand();
	}

	int get_rand() {
		return rand();
	}

	int get_mouseX() {
		return mouseX;
	}

	int get_mouseY() {
		return mouseY;
	}

	void reset_key_state() {
		memcpy(prev_key_state, key_state, sizeof(key_state));
		GetKeyboardState(key_state);
	}

	bool get_key_state(int key) {
		if (key_state[key] & 0x80)
			return true;
		return false;
	}

	bool get_key_down_state(int key) {
		if ((key_state[key] & 0x80) && !(prev_key_state[key] & 0x80))
			return true;
		return false;
	}
}