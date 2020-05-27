#include "Map.h"
#include "DirectXModule.h"
#include "WindowsModule.h"
#include "Field.h"
#include "Pub.h"
#include "Menu.h"
#include "general.h"
#include "sqlite3.h"
#include <string>
#include "TalkWindow.h"
#include "ImageManager.h"
#include "Player.h"

map::map() {
	this->map_bitmap = image_manager::get_image_id(L"image/map.png");
	this->map_point_bitmap = image_manager::get_image_id(L"image/map_point.png");
	this->window_bitmap = image_manager::get_image_id(L"image/window.png");
	this->bitmap_change_count = 0;
	this->bitmap_num = 0;
	this->chosen_point = 0;
	this->unlock();
	game::get_incetance().set_sound(0);

	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	sqlite3_stmt* stage_stmt = nullptr;
	sqlite3_prepare(db, "SELECT * FROM map", -1, &stage_stmt, nullptr);
	while (sqlite3_step(stage_stmt) == SQLITE_ROW) {
		map_stage_inf* inf_buf = new map_stage_inf();
		inf_buf->stage_name = genmod::string_to_wstring((char*)sqlite3_column_text(stage_stmt, 1));
		inf_buf->stage_exp = genmod::string_to_wstring((char*)sqlite3_column_text(stage_stmt, 2));
		inf_buf->x = sqlite3_column_int(stage_stmt, 3);
		inf_buf->y = sqlite3_column_int(stage_stmt, 4);
		this->stage_inf.push_back(inf_buf);
	}
}

map::~map() {
	image_manager::release_image(this->map_bitmap);
	image_manager::release_image(this->map_point_bitmap);
	for (int i = 0; i < this->stage_inf.size(); i++) {
		delete this->stage_inf[i];
	}
	this->stage_inf.clear();
}

void map::calc() {

	int talk_event_id = game::get_incetance().get_talk_event();
	if (talk_event_id != -1) {
		game::get_incetance().stack_scene(new talk_window(this,talk_event_id));
	}
	if (++this->bitmap_change_count > 10) {
		this->bitmap_change_count = 0;
		this->bitmap_num = (this->bitmap_num + 1) % 4;
	}

	if (winmod::get_key_down_state(VK_LEFT) && this->chosen_point > 0) {
		this->chosen_point--;
	}
	if (winmod::get_key_down_state(VK_RIGHT) && this->chosen_point < this->stage_inf.size()-1) {
		this->chosen_point++;
	}
	if (winmod::get_key_down_state('Z')) {
		game::get_incetance().erase_scene(this);
		if (this->chosen_point == 0) {
			stage* s1_1 = new field(0);
			s1_1->enter_stage(50,600);
			game::get_incetance().stack_scene(s1_1);
		}
		else if (this->chosen_point == 1) {
			stage* s1_1 = new pub();
			s1_1->enter_stage(50, 400);
			game::get_incetance().stack_scene(s1_1);
		}
	}
	else if (winmod::get_key_down_state('X')) {
		game::get_incetance().stack_scene(new menu(this));
	}
}

void map::draw() {
	dxmod::d2d_draw_bitmapUL(this->map_bitmap, 0, 0);
	for (int i = 0; i < this->stage_inf.size(); i++) {
		if (this->chosen_point == i) {
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->map_point_bitmap, this->stage_inf[i]->x-25, this->stage_inf[i]->y-25, 50, 50, this->bitmap_num, 0);
		}
		else {
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->map_point_bitmap, this->stage_inf[i]->x-25, this->stage_inf[i]->y-25, 50, 50, 0, 0);
		}
	}
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 5, 0, 0, 150, 25);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 30, 0, 125, 150, 25);
	dxmod::d2d_draw_string(10, 10, this->stage_inf[this->chosen_point]->stage_name);
}