#include "Field.h"
#include "DirectXModule.h"
#include "WindowsModule.h"
#include "general.h"
#include "sqlite3.h"
#include "Player.h"
#include "Application.h"
#include "TalkWindow.h"
#include "Menu.h"
#include "EventObj.h"
#include <sstream>
#include "ImageManager.h"
#include "StageChanger.h"

field::field(int stage_id) :stage(stage_id){
	this->map_chip = image_manager::get_image_id(L"image/mapchip0.png");
	this->background_bitmap = image_manager::get_image_id(L"image/background0.png");
	this->back_sky_bitmap = image_manager::get_image_id(L"image/back_sky0.png");
	
	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	sqlite3_stmt* changer_stmt = nullptr;
	std::stringstream changer_exec;
	changer_exec << "SELECT * FROM Stage_changer_pos WHERE origin_scene_id = " << stage_id;
	sqlite3_prepare(db, changer_exec.str().c_str(), -1, &changer_stmt, nullptr);
	while (sqlite3_step(changer_stmt) == SQLITE_ROW) {
		int next_stage_id = sqlite3_column_int(changer_stmt, 1);
		int x = sqlite3_column_int(changer_stmt, 2);
		int y = sqlite3_column_int(changer_stmt, 3);
		int dist_x = sqlite3_column_int(changer_stmt, 4);
		int dist_y = sqlite3_column_int(changer_stmt, 5);
		stage_changer* tmp = new field_stage_changer(x, y, this, next_stage_id, dist_x, dist_y);
		this->changer.push_back(tmp);
		this->objects.insert(this->objects.begin(),tmp);
	}
}

field::~field() {
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	objects.clear();
	image_manager::release_image(this->background_bitmap);
	image_manager::release_image(this->back_sky_bitmap);
	image_manager::release_image(this->map_chip);
	for (int i = 0; i < this->map_line; i++)
		delete[] map_data[i];
	delete[] map_data;
}

void field::calc() {
	int talk_event_id = game::get_incetance().get_talk_event();
	if (talk_event_id != -1) {
		game::get_incetance().stack_scene(new talk_window(this, talk_event_id));
	}

	player::get_incetance()->calc();

	if (winmod::get_key_down_state('X')) {
		game::get_incetance().stack_scene(new menu(this));
	}

	for (auto itr : objects) {
		itr->calc();
	}
}

void field::draw() {
	//”wŒi•`‰æ
	dxmod::d2d_draw_bitmapUL(this->back_sky_bitmap, 0, 0);
	dxmod::d2d_draw_bitmapUL(this->background_bitmap, (-1.f / 10.f) * camera_x, 0);

	//ƒ}ƒbƒv•`‰æ
	for (int i = 0; i < 7; i++) {
		for (int j = 0; j < 9; j++) {
			if (this->map_data[i + (this->camera_y / 100)][j + (this->camera_x / 100)] == 1) {
				dxmod::d2d_draw_part_of_bitmap_NumUL(this->map_chip, -this->camera_x % 100 + 100 * j, -this->camera_y % 100 + 100 * i, 100, 100, 3, 1);
			}
			else if (this->map_data[i + (this->camera_y / 100)][j + (this->camera_x / 100)] == 2) {
				dxmod::d2d_draw_part_of_bitmap_NumUL(this->map_chip, -this->camera_x % 100 + 100 * j, -this->camera_y % 100 + 100 * i, 100, 100, 0, 1);
			}
			else if (this->map_data[i + (this->camera_y / 100)][j + (this->camera_x / 100)] == 3) {
				dxmod::d2d_draw_part_of_bitmap_NumUL(this->map_chip, -this->camera_x % 100 + 100 * j, -this->camera_y % 100 + 100 * (i - 1), 100, 200, 1, 0);
			}
			else if (this->map_data[i + (this->camera_y / 100)][j + (this->camera_x / 100)] == 4) {
				dxmod::d2d_draw_part_of_bitmap_NumUL(this->map_chip, -this->camera_x % 100 + 100 * j, -this->camera_y % 100 + 100 * (i - 1), 100, 200, 2, 0);
			}
		}
	}
	//ƒIƒuƒWƒFƒNƒg•`‰æ

	for (auto itr : objects) {
		itr->draw(camera_x, camera_y);
	}

	player::get_incetance()->draw(this->camera_x, this->camera_y);

	dxmod::d2d_draw_string(10, 10, L"HP:");
	dxmod::d2d_draw_rect(0, 50, 20, player::get_incetance()->get_maxhp() + player::get_incetance()->get_skill_maxhp() + 50, 25, 1, false);
	dxmod::d2d_draw_rect_with_dynamicscb(1, 1, 1, 0.7, player::get_incetance()->get_hp() + 50, 20, 50, 25);
}