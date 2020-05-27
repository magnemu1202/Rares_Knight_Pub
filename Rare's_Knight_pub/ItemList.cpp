#include "ItemList.h"
#include "DirectXModule.h"
#include "WindowsModule.h"
#include "Application.h"
#include "ImageManager.h"
#include "Player.h"
#include "Item.h"
#include <cmath>
#include <sstream>

item_list::item_list(scene* origin) {
	this->back_ground_brush = dxmod::d2d_create_brush(0.f, 0.f, 0.f, 0.5f);
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 0.5f);
	this->hp_bar_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 1.f);
	this->main_text_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 1.f);
	this->main_text_font = dxmod::d2d_create_font(L"メイリオ", 32.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);
	this->window_bitmap = image_manager::get_image_id(L"image/window.png");
	this->player_bitmap = image_manager::get_image_id(L"image/stay.png");
	this->cursol = 0;
	this->top = 0;
	this->count = 0;
	this->origin = origin;
	this->origin->lock();
	this->update_exist_items();
}

item_list::~item_list() {
	this->exist_items.clear();
	dxmod::d2d_release_brush(this->back_ground_brush);
	dxmod::d2d_release_brush(this->hp_bar_brush);
	dxmod::d2d_release_brush(this->main_text_brush);
	dxmod::d2d_release_font(this->main_text_font);
	image_manager::release_image(this->window_bitmap);
	image_manager::release_image(this->player_bitmap);
}

void item_list::update_exist_items() {
	this->exist_items.clear();
	for (item* itr : player::get_incetance()->get_items()) {
		if (itr->get_num() > 0) {
			this->exist_items.push_back(itr);
		}
	}
}

void item_list::calc() {
	dxmod::d2d_release_brush(this->cursol_brush);
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 0.25 * std::sin(0.1 * (++this->count)) + 0.5);

	if (winmod::get_key_down_state(VK_UP) && this->cursol > 0) {
		this->cursol--;
	}
	if (winmod::get_key_down_state(VK_DOWN) && this->cursol < this->exist_items.size() - 1) {
		this->cursol++;
	}

	if (winmod::get_key_down_state('X')) {
		game::get_incetance().erase_scene(this);
		this->origin->unlock();
	}
	else if (winmod::get_key_down_state('Z') && this->cursol < this->exist_items.size()) {
		exist_items[this->cursol]->use();
		this->update_exist_items();
		if (this->cursol >= this->exist_items.size()) {
			this->cursol = this->exist_items.size()-1;
		}
	}
}

void item_list::draw() {
	dxmod::d2d_draw_rect(this->back_ground_brush, 0, 0, 800, 600, 0, true);
	
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 10, 0, 0, 100, 150);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 105, 10, 25, 0, 100, 150);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 205, 10, 50, 0, 100, 150);

	dxmod::d2d_draw_custom_text_string(this->main_text_font, this->main_text_brush, 100, 100, L"アイテム");

	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 200, 0, 0, 100, 50);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 105, 200, 25, 0, 100, 50);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 205, 200, 50, 0, 100, 50);
	for (int i = 0; i < 5; i++) {
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 250 + 50 * i, 0, 50, 100, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 105, 250 + 50 * i, 25, 50, 100, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 205, 250 + 50 * i, 50, 50, 100, 50);
	}
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 500, 0, 100, 100, 50);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 105, 500, 25, 100, 100, 50);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 205, 500, 50, 100, 100, 50);

	for (int i = 0; i < this->top + 6 && i < this->exist_items.size(); i++) {
		dxmod::d2d_draw_string(10, 215 + 30 * i, this->exist_items[i + this->top]->get_name());
	}

	dxmod::d2d_draw_rect(this->cursol_brush, 7, 207 + 50 * (this->cursol - this->top), 302, 252 + 50 * (this->cursol - this->top), 0, true);

	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 355, 10, 0, 0, 100, 150);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 455, 10, 25, 0, 100, 150);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 555, 10, 25, 0, 100, 150);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 655, 10, 50, 0, 100, 150);

	dxmod::d2d_draw_part_of_bitmapUL(this->player_bitmap, 350, 200, 0, 0, 200, 200);
	std::wstringstream status_disp;
	status_disp << L"HP : " << player::get_incetance()->get_hp() << L" / " << player::get_incetance()->get_maxhp() + player::get_incetance()->get_skill_maxhp();
	dxmod::d2d_draw_string(350, 420, status_disp.str().c_str());
	
	dxmod::d2d_draw_rect(this->hp_bar_brush, 350, 450, player::get_incetance()->get_maxhp() + player::get_incetance()->get_skill_maxhp() + 350, 465, 1, false);
	dxmod::d2d_draw_rect(this->hp_bar_brush, 350, 450, player::get_incetance()->get_hp() + 350, 465, 0, true);

	status_disp.str(L"");
	status_disp << L"ATK: " << player::get_incetance()->get_atk();
	dxmod::d2d_draw_string(560, 220, status_disp.str().c_str());
	status_disp.str(L"");
	status_disp << L"DEF: " << player::get_incetance()->get_def();
	dxmod::d2d_draw_string(560, 250, status_disp.str().c_str());
	status_disp.str(L"");
	status_disp << L"SPD: " << player::get_incetance()->get_spd();
	dxmod::d2d_draw_string(560, 280, status_disp.str().c_str());
}