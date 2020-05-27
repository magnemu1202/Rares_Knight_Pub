#include "DirectXModule.h"
#include "WindowsModule.h"
#include "Title.h"
#include "Map.h"
#include "Application.h"
#include "SaveData.h"
#include "BlankScene.h"
#include <memory>
#include <cmath>
#include "ImageManager.h"
extern HWND hWnd;

title::title() {
	this->cursol = 0;
	this->cursol_effect_counter = 0;
	this->bitmap = image_manager::get_image_id(L"image/title.png");
	this->window_bitmap = image_manager::get_image_id(L"image/window.png");
	this->text_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 1.f);
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 0.5f);
	this->text_font = dxmod::d2d_create_font(L"メイリオ", 24.0f, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);
	this->unlock();
}

title::~title() {
	image_manager::release_image(this->bitmap);
	image_manager::release_image(this->window_bitmap);
	dxmod::d2d_release_brush(this->text_brush);
	dxmod::d2d_release_font(this->text_font);
}

void title::calc() {
	dxmod::d2d_release_brush(this->cursol_brush);
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, (0.25 * std::sin((++this->cursol_effect_counter) / 30.f)) + 0.5);
	if (winmod::get_key_down_state(VK_UP) && this->cursol >= 1) {
		this->cursol--;
	}
	if (winmod::get_key_down_state(VK_DOWN) && this->cursol < 3) {
		this->cursol++;
	}


	if (winmod::get_key_down_state('Z')) {
		if (this->cursol == 0) {
			game::get_incetance().stack_scene(new blank_scene(this, new map(), 60));
			game::get_incetance().stack_talk_event(0);
		}
		else if (this->cursol == 1) {
			game::get_incetance().stack_scene(new load_datas(this));
		}
		else
			PostMessage(hWnd, WM_CLOSE, 0, 0);
	}
}

void title::draw() {	
	dxmod::d2d_draw_bitmapUL(this->bitmap, 0, 0);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 400, 0, 0, 100, 150);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 105, 400, 50, 0, 100, 150);
	dxmod::d2d_draw_custom_text_string(this->text_font, this->text_brush, 10, 410, L"ニューゲーム");
	dxmod::d2d_draw_custom_text_string(this->text_font, this->text_brush, 10, 460, L"ロードゲーム");
	dxmod::d2d_draw_custom_text_string(this->text_font, this->text_brush, 10, 510, L"ゲーム終了");
	dxmod::d2d_draw_rect(this->cursol_brush, 10, 400 + this->cursol * 50, 200, 450 + this->cursol * 50, 0, true);
}