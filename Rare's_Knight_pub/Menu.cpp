#include "Menu.h"
#include "DirectXModule.h"
#include "WindowsModule.h"
#include "Scene.h"
#include "Application.h"
#include "SkillTree.h"
#include "SaveData.h"
#include "Title.h"
#include "ItemList.h"
#include <cmath>
#include "ImageManager.h"

menu::menu(scene* origin) {
	this->cursol = 0;
	this->cursol_effect_counter = 0;
	this->origin = origin;
	this->origin->lock();
	this->back_ground_brush = dxmod::d2d_create_brush(0.f, 0.f, 0.f, 0.5f);
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 0.5f);
	this->menu_font = dxmod::d2d_create_font(L"メイリオ", 16.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);
	this->main_text_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 1.f);
	this->main_text_font = dxmod::d2d_create_font(L"メイリオ", 32.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);
	this->window_bitmap = image_manager::get_image_id(L"image/window.png");
	game::get_incetance().set_sound(1);
}

menu::~menu() {
	dxmod::d2d_release_brush(this->back_ground_brush);
	dxmod::d2d_release_brush(this->cursol_brush);
	dxmod::d2d_release_brush(this->main_text_brush);
	dxmod::d2d_release_font(this->menu_font);
	dxmod::d2d_release_font(this->main_text_font);
	image_manager::release_image(this->window_bitmap);
}

void menu::calc() {
	dxmod::d2d_release_brush(this->cursol_brush);
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, (0.25 * std::sin((++this->cursol_effect_counter) / 30.f)) + 0.5);
	if (winmod::get_key_down_state('X')) {
		this->origin->unlock();
		game::get_incetance().erase_scene(this);
		game::get_incetance().set_sound(1);
	}
	else if (winmod::get_key_down_state('Z')) {
		if (this->cursol == 0) {
			game::get_incetance().stack_scene(new item_list(this));
		}
		else if (this->cursol == 1) {
			game::get_incetance().stack_scene(new skill_tree(this));
		}
		else if (this->cursol == 2) {
			game::get_incetance().stack_scene(new save_datas(this));
		}
		else if (this->cursol == 3) {
			game::get_incetance().reset_sound();
			game::get_incetance().erase_scene(this);
			game::get_incetance().erase_scene(this->origin);
			game::get_incetance().stack_scene(new title());
			game::get_incetance().set_sound(1);
		}
	}
	if (winmod::get_key_down_state(VK_UP)) {
		this->cursol = (this->cursol - 1) % 4;
		if (this->cursol < 0) {
			this->cursol = 3;
		}
	}
	else if (winmod::get_key_down_state(VK_DOWN)) {
		this->cursol = (this->cursol + 1) % 4;
	}
}

void menu::draw() {
	dxmod::d2d_draw_rect(this->back_ground_brush, 0, 0, 800, 600, 0, true);

	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 100, 0, 0, 100, 50);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 105, 100, 25, 0, 100, 50);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 205, 100, 50, 0, 100, 50);
	for (int i = 0; i < 3; i++) {
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 150 + 50 * i, 0, 50, 100, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 105, 150 + 50 * i, 25, 50, 100, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 205, 150 + 50 * i, 50, 50, 100, 50);
	}
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 300, 0, 100, 100, 50);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 105, 300, 25, 100, 100, 50);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 205, 300, 50, 100, 100, 50);

	dxmod::d2d_draw_custom_text_string(this->menu_font, this->menu_brush, 10, 115, L"アイテム");
	dxmod::d2d_draw_custom_text_string(this->menu_font, this->menu_brush, 10, 165, L"スキル");
	dxmod::d2d_draw_custom_text_string(this->menu_font, this->menu_brush, 10, 215, L"セーブ");
	dxmod::d2d_draw_custom_text_string(this->menu_font, this->menu_brush, 10, 265, L"タイトルに戻る");
	dxmod::d2d_draw_rect(this->cursol_brush, 5, this->cursol * 50 + 100, 300, this->cursol * 50 + 150, 0, true);
}