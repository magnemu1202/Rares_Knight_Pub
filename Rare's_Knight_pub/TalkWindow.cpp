#include "TalkWindow.h"
#include "Application.h"
#include "DirectXModule.h"
#include "WindowsModule.h"
#include "general.h"
#include "sqlite3.h"
#include <sstream>
#include "ImageManager.h"

talk_window::talk_window(scene* origin,int talk_id) {
	this->origin = origin;
	this->origin->lock();
	this->window_bitmap = image_manager::get_image_id(L"image/window.png");
	this->text_brush = dxmod::d2d_create_brush(1, 1, 1, 1);
	this->text_font = dxmod::d2d_create_font(L"ÉÅÉCÉäÉI", 28.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);

	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	sqlite3_stmt* talk_stmt = nullptr;
	std::stringstream ss;
	ss << "SELECT * FROM Talk WHERE talk_id = " << talk_id;
	sqlite3_prepare(db, ss.str().c_str(), -1, &talk_stmt, nullptr);
	while (sqlite3_step(talk_stmt) == SQLITE_ROW) {
		std::wstringstream talk_text;
		talk_text << genmod::string_to_wstring((char*)sqlite3_column_text(talk_stmt, 2)) << std::endl;
		talk_text << genmod::string_to_wstring((char*)sqlite3_column_text(talk_stmt, 3)) << std::endl;
		talk_text << genmod::string_to_wstring((char*)sqlite3_column_text(talk_stmt, 4));
		this->talk.push_back(talk_text.str().c_str());
	}
}

talk_window::~talk_window() {
	image_manager::release_image(this->window_bitmap);
	dxmod::d2d_release_font(this->text_font);
	dxmod::d2d_release_brush(this->text_brush);
	for (int i = 0; i < this->talk.size(); i++) {
		this->talk[i].clear();
	}
	this->talk.clear();
}

void talk_window::calc() {
	if (winmod::get_key_down_state('Z')) {
		this->talking_itr++;
	}
	if (this->talking_itr >= this->talk.size()) {
		game::get_incetance().erase_scene(this);
		this->origin->unlock();
	}
}

void talk_window::draw() {
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 50, 400, 0, 0, 50, 150);
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 700, 400, 100, 0, 50, 150);
	for (int i = 0; i < 12; i++) {
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 100 + i * 50, 400, 50, 0, 50, 150);
	}
	if (this->talking_itr < this->talk.size()) {
		dxmod::d2d_draw_custom_text_string(this->text_font, this->text_brush, 55, 405, this->talk[this->talking_itr]);
	}
}