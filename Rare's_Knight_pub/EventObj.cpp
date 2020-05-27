#include "EventObj.h"
#include <vector>
#include <sstream>
#include "sqlite3.h"
#include "WindowsModule.h"
#include "DirectXModule.h"
#include "Application.h"
#include "Player.h"
#include "Field.h"
#include "Map.h"
#include "BlankScene.h"
#include "ImageManager.h"

event_obj* create_event(int event_id) {
	if (event_id == 0) {
		return new event0();
	}
	else if (event_id == 1) {
		return new paper_map();
	}
}

void event_obj::talk() {
	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	sqlite3_stmt* event_talk_stmt = nullptr;
	std::stringstream event_talk_exec;
	event_talk_exec << "select * from Event_talk where event_id = " << this->id;
	sqlite3_prepare(db, event_talk_exec.str().c_str(), -1, &event_talk_stmt, nullptr);
	while (sqlite3_step(event_talk_stmt) == SQLITE_ROW) {
		game::get_incetance().stack_talk_event(sqlite3_column_int(event_talk_stmt, 1));
	}
}

event0::event0() {
	this->id = 0;
	this->bitmap = image_manager::get_image_id(L"image/parson0.png");

}

event0::~event0() {
	image_manager::release_image(this->bitmap);
}

void event0::calc() {

}

void event0::draw(int camera_x,int camera_y) {
	dxmod::d2d_draw_bitmap(this->bitmap,camera_x - this->x, camera_y - this->y);
}

paper_map::paper_map() {
	this->id = 1;
	this->bitmap = image_manager::get_image_id(L"image/paper_map.png");
}

paper_map::~paper_map() {
	image_manager::release_image(this->bitmap);
}

void paper_map::calc() {

}

void paper_map::draw(int camera_x,int camera_y) {
	dxmod::d2d_draw_bitmap(this->bitmap, camera_x - this->x, camera_y - this->y);
}