#pragma once
#include <windows.h>
#include <locale.h>
#include <dwrite.h>
#include <d2d1.h>
#include <d2d1helper.h>
#include <d3d10_1.h>
#include <wingdi.h>
#include <wincodec.h>
#include <dsound.h>
#include <string>

#pragma comment(lib,"d3d10_1.lib")
#pragma comment(lib,"d2d1.lib")
#pragma comment(lib,"dwrite.lib")
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "gdi32.lib")

namespace dxmod {
	class LTimer {
	public:
		bool set;
		unsigned int SetTimer;
		unsigned int AddTimer;
		LTimer() {
			set = true;
		}
	};
	void dx_end();
	void set_VSync();
	void reset_VSync();
	int set_time_point();
	unsigned int get_timer(int TimerID);
	void reset_timer(int TimerID);
	int dx_init();
	void d2d_start_draw();
	void d2d_end_draw();
	void d2d_full_screen(bool FullScreen);
	void d2d_fill_window(float R, float G, float B, float alpha);
	void d2d_draw_FPSRate(float x, float y);
	void all_dxdevice_Release();
	int d2d_load_png_from_resource(LPCSTR ResourceName);
	int d2d_load_png_from_file(LPCWSTR FileName);
	int d2d_create_brush(float Red, float Green, float Blue, float Alpha);
	int d2d_create_rg_brush(float Red1, float Green1, float Blue1, float Alpha1, float Red2, float Green2, float Blue2, float Alpha2, float StartX, float StartY, float EndX, float EndY);
	int d2d_create_lg_brush(float Red1, float Green1, float Blue1, float Alpha1, float Red2, float Green2, float Blue2, float Alpha2, float StartX, float StartY, float EndX, float EndY);
	int d2d_create_bitmap_brush(int BitmapID);
	int d2d_create_font(LPCWSTR FontName, float FontSize, DWRITE_FONT_WEIGHT FontWeight, DWRITE_FONT_STYLE FontStyle, DWRITE_FONT_STRETCH FontStretch);
	int d2d_set_layer(float left, float top, float right, float bottom);
	void d2d_pop_layer();
	void d2d_set_translation(float x, float y);
	void d2d_set_rotate(float centerx, float centery, float Rotate);
	void d2d_set_scale(float centerx, float centery, float width, float height);
	void d2d_set_skew(float centerx, float centery, float anglex, float angley);
	void d2d_set_reversex(float centerx);
	void d2d_reset_transform();
	void d2d_draw_bitmapUL(int BitmapID, float leftx, float topy);
	void d2d_draw_bitmap(int BitmapID, float x, float y);
	void d2d_draw_rotate_bitmap(int BitmapID, float x, float y, float Rotate);
	void d2d_draw_xreversed_bitmap(int BitmapID, float x, float y);
	void d2d_draw_part_of_bitmapUL(int BitmapID, float x, float y, float Bitmapx, float Bitmapy, float BitmapWidth, float BitmapHeight);
	void d2d_draw_part_of_bitmap_NumUL(int BitmapID, float x, float y, float BitmapWidth, float BitmapHeight, int xNum, int yNum);
	void d2d_draw_part_of_xreversed_bitmapUL(int BitmapID, float x, float y, float Bitmapx, float Bitmapy, float BitmapWidth, float BitmapHeight);
	void d2d_draw_part_of_xreversed_bitmap_NumUL(int BitmapID, float x, float y, float BitmapWidth, float BitmapHeight, int xNum, int yNum);
	void d2d_draw_string(float x, float y, std::wstring DrawStr);
	void d2d_draw_custom_text_string(int FontID, int BrushID, float x, float y, std::wstring DrawStr);
	void d2d_draw_line(int BrushID, float startx, float starty, float endx, float endy, float StrokeWidth = 1.0);
	void d2d_draw_circle(int BrushID, float Centerx, float Centery, float Width, float Height, float StrokeWidth, bool fill);
	void d2d_draw_rect(int BrushID, int left, int top, int right, int bottom, float StrokeWidth, bool fill);
	int d2d_draw_rect_with_dynamicscb(float Red, float Green, float Blue, float Alpha, float right, float top, float left, float bottom);
	int d2d_draw_rect_with_dynamiclb(float Red1, float Green1, float Blue1, float Alpha1, float Red2, float Green2, float Blue2, float Alpha2, float StartX, float StartY, float EndX, float EndY, float right, float top, float left, float bottom);
	void d2d_release_bitmap(int BitmapID);
	void d2d_release_font(int FontID);
	void d2d_release_brush(int BrushID);
	int ds_load_wave_from_file(LPSTR FileName);
	//int dsLoadWaveFromResource(LPCSTR ResourceName);
	void ds_sound_play(int SoundID);
	int ds_get_current_cursor(int SoundID);
	void ds_set_freqency(int SoundID, int SoundFreq);
	void ds_set_volume(int SoundID, long SoundVol);
	void ds_sound_stop(int SoundID);
	void ds_sound_set_pos(int SoundID, int StartPosition);
	void ds_release_wave(int SoundID);
}