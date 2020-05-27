#include "SaveData.h"
#include "Application.h"
#include "WindowsModule.h"
#include "DirectXModule.h"
#include <sstream>
#include "ImageManager.h"

save_datas::save_datas(scene* origin) {
	this->window_bitmap = image_manager::get_image_id(L"image/window.png");
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 1.f);
	this->cursol_effect_counter = 0;
	this->origin = origin;
	this->origin->lock();
	for (int i = 0; i < 5; i++) {
		file_is_normal[i] = game::get_incetance().check_save_data(i);
	}
}

save_datas::~save_datas() {
	image_manager::release_image(this->window_bitmap);
	dxmod::d2d_release_brush(this->cursol_brush);
}

void save_datas::calc() {
	dxmod::d2d_release_brush(this->cursol_brush);
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, (0.25 * std::sin((++this->cursol_effect_counter) / 30.f)) + 0.5);
	if (winmod::get_key_down_state(VK_UP) && this->cursor > 0) {
		this->cursor--;
	}
	else if (winmod::get_key_down_state(VK_DOWN) && this->cursor < 4) {
		this->cursor++;
	}
	if (winmod::get_key_down_state('Z')) {
		game::get_incetance().save_data(this->cursor);
		game::get_incetance().erase_scene(this);
		this->origin->unlock();
	}
	else if (winmod::get_key_down_state('X')) {
		game::get_incetance().erase_scene(this);
		this->origin->unlock();
	}
}

void save_datas::draw() {
	for (int i = 0; i < 5; i++) {
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 300, 50 + i * 110, 0, 0, 50, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 300, 100 + i * 110, 0, 100, 50, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 650, 50 + i * 110, 100, 0, 50, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 650, 100 + i * 110, 100, 100, 50, 50);
		for (int j = 0; j < 6; j++) {
			dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 350 + j * 50, 50 + i * 110, 50, 0, 50, 50);
			dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 350 + j * 50, 100 + i * 110, 50, 100, 50, 50);
		}
		if (this->file_is_normal[i]) {
			std::wstringstream save_data_name;
			save_data_name << L"Save " << i;
			dxmod::d2d_draw_string(310, 60 + i * 110, save_data_name.str().c_str());
		}
		else {
			dxmod::d2d_draw_string(310, 60 + i * 110, L"No data");
		}
	}
	dxmod::d2d_draw_rect(this->cursol_brush, 300, 50 + this->cursor * 110, 700, 150 + this->cursor * 110, 0, true);
}

load_datas::load_datas(scene* origin) {
	this->window_bitmap = image_manager::get_image_id(L"image/window.png");
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 1.f);
	this->cursol_effect_counter = 0;
	this->origin = origin;
	this->origin->lock();
	for (int i = 0; i < 5; i++) {
		file_is_normal[i] = game::get_incetance().check_save_data(i);
	}
}

load_datas::~load_datas() {
	image_manager::release_image(this->window_bitmap);
	dxmod::d2d_release_brush(this->cursol_brush);
}

void load_datas::calc() {
	dxmod::d2d_release_brush(this->cursol_brush);
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, (0.25 * std::sin((++this->cursol_effect_counter) / 30.f)) + 0.5);
	if (winmod::get_key_down_state(VK_UP) && this->cursor > 0) {
		this->cursor--;
	}
	else if (winmod::get_key_down_state(VK_DOWN) && this->cursor < 4) {
		this->cursor++;
	}
	if (winmod::get_key_down_state('Z') && file_is_normal[this->cursor]) {
		game::get_incetance().load_data(this->cursor);
		game::get_incetance().load_game();
		this->origin->unlock();
	}
	else if (winmod::get_key_down_state('X')) {
		game::get_incetance().erase_scene(this);
		this->origin->unlock();
	}
}

void load_datas::draw() {
	for (int i = 0; i < 5; i++) {
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 100, 50 + i * 110, 0, 0, 50, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 100, 100 + i * 110, 0, 100, 50, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 450, 50 + i * 110, 100, 0, 50, 50);
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 450, 100 + i * 110, 100, 100, 50, 50);
		for (int j = 0; j < 6; j++) {
			dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 150 + j * 50, 50 + i * 110, 50, 0, 50, 50);
			dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 150 + j * 50, 100 + i * 110, 50, 100, 50, 50);
		}
		if (this->file_is_normal[i]) {
			std::wstringstream save_data_name;
			save_data_name << L"Save" << i;
			dxmod::d2d_draw_string(110, 60 + i * 110, save_data_name.str().c_str());
		}
		else {
			dxmod::d2d_draw_string(110, 60 + i * 110, L"No data");
		}
	}
	dxmod::d2d_draw_rect(this->cursol_brush, 100, 50 + this->cursor * 110, 500, 150 + this->cursor * 110, 0, true);
}