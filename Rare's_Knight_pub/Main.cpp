#include "WindowsModule.h"
#include "DirectXModule.h"
#include "Application.h"
extern HWND hWnd = nullptr;

#pragma comment(lib,"sqlite3.lib")

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wcex;
	winmod::app_init(&wcex,hInstance,800.0,600.0);
	dxmod::dx_init();
	winmod::set_rand_seed();
	dxmod::d2d_full_screen(false);
	MSG hMsg = {0};

	while (hMsg.message != WM_QUIT) {
		if (PeekMessageW(&hMsg, NULL, 0, 0, PM_REMOVE)) {
			TranslateMessage(&hMsg);
			DispatchMessage(&hMsg);
		}
		game::get_incetance().calculation();
		game::get_incetance().draw();
	}
	
	return 0;
}