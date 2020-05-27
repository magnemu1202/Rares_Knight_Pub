#pragma once
#include <windows.h>
#include <wingdi.h>
#include <wincodec.h>
#include <locale.h>
#include <time.h>

namespace winmod {
	void app_init(WNDCLASS* wcex, HINSTANCE hInstance, float WindowWidth, float WindowHeight);
	void win_end();
	int win_init(WNDCLASS* wcex, HINSTANCE hInstance, float WindowWidth, float WindowHeight);
	bool set_window_size(int x, int y);
	bool set_window_pos(int x, int y);
	bool set_title(LPCWSTR TitleText);
	void load_tabled_stringW(UINT StringID, LPWSTR* GetStr, int size);
	void load_tabled_String(UINT StringID, LPSTR* GetStr, int size);
	void set_rand_seed();
	int get_rand();
	int get_mouseX();
	int get_mouseY();
	void reset_key_state();
	bool get_key_state(int key);
	bool get_key_down_state(int key);
};