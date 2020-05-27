#include "Item.h"
#include <sstream>
#include "Application.h"
#include "DirectXModule.h"
#include "general.h"
#include "sqlite3.h"
#include "ImageManager.h"

item* create_item(int item_id) {
	if (item_id == 0) {
		return new item0(item_id);
	}
}

item::item(int id) {
	this->id = id;
	this->bitmap = -1;
	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	std::stringstream item_exec;
	item_exec << "SELECT * FROM Item WHERE id = " << id;
	sqlite3_stmt* item_stmt = nullptr;
	sqlite3_prepare(db, item_exec.str().c_str(), -1, &item_stmt, nullptr);
	if (sqlite3_step(item_stmt) == SQLITE_ROW) {
		this->name = genmod::string_to_wstring((char*)sqlite3_column_text(item_stmt, 1));
		this->description = genmod::string_to_wstring((char*)sqlite3_column_text(item_stmt, 2));
		this->value = sqlite3_column_int(item_stmt, 4);
	}
	sqlite3_reset(item_stmt);
	sqlite3_finalize(item_stmt);
	sqlite3_close(db);
}

void item::load_bitmap() {
	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	std::stringstream item_exec;
	item_exec << "SELECT * FROM Item WHERE id = " << this->id;
	sqlite3_stmt* item_stmt = nullptr;
	sqlite3_prepare(db, item_exec.str().c_str(), -1, &item_stmt, nullptr);
	if (sqlite3_step(item_stmt) == SQLITE_ROW) {
		this->bitmap = image_manager::get_image_id(genmod::string_to_wstring((char*)sqlite3_column_text(item_stmt, 3)));
	}
	sqlite3_reset(item_stmt);
	sqlite3_finalize(item_stmt);
	sqlite3_close(db);
}

void item::release_bitmap() {
	image_manager::release_image(this->bitmap);
}

item0::item0(int id) : item(id) {

}

item0::~item0() {
	image_manager::release_image(this->bitmap);
}

void item0::use() {
	player::get_incetance()->set_hp(50);
}



