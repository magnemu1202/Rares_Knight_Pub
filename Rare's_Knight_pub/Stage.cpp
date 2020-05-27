#include "Stage.h"
#include "sqlite3.h"
#include <fstream>
#include <sstream>
#include <cstdio>
#include <iostream>
#include "DirectXModule.h"
#include "WindowsModule.h"
#include "general.h"
#include "GameObj.h"
#include "Enemy.h"
#include "Application.h"
#include "Player.h"
#include "Menu.h"
#include "TalkWindow.h"

stage::stage(int stage_id) {

	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	std::stringstream stage_exec;
	stage_exec << "SELECT * FROM Stage WHERE id = " << stage_id;
	sqlite3_stmt* stage_stmt = nullptr;
	sqlite3_prepare(db, stage_exec.str().c_str(), -1, &stage_stmt, nullptr);
	if (sqlite3_step(stage_stmt) == SQLITE_ROW) {
		std::stringstream stream_buf;
		std::string buf;

		stream_buf << (char*)sqlite3_column_text(stage_stmt, 1);
		char** map_data;
		int column_num = 0;

		int line_num = 0;
		while (std::getline(stream_buf, buf)) {
			line_num++;
		}
		stream_buf.clear();
		stream_buf.seekg(0, std::ios_base::beg);
		std::getline(stream_buf, buf);
		column_num = buf.length();
		map_data = new char* [line_num];
		for (int i = 0; i < line_num; i++) {
			map_data[i] = new char[column_num];
		}
		stream_buf.clear();
		stream_buf.seekg(0, std::ios_base::beg);
		this->map_column = column_num;
		this->map_line = line_num;

		//マップデータを初期化


		int data_line = 0;
		while (std::getline(stream_buf, buf)) {
			for (int i = 0; i < column_num; i++) {
				map_data[data_line][i] = buf[i] - '0';
			}
			data_line++;
		}
		this->map_data = map_data;
	}
	sqlite3_reset(stage_stmt);
	sqlite3_finalize(stage_stmt);

	//敵読み込み
	std::stringstream enemy_pos_exec;
	enemy_pos_exec << "SELECT * FROM Enemy_pos WHERE stage_id = " << stage_id;
	sqlite3_stmt* enemy_pos_stmt = nullptr;
	sqlite3_prepare(db, enemy_pos_exec.str().c_str(), -1, &enemy_pos_stmt, nullptr);
	while (sqlite3_step(enemy_pos_stmt) == SQLITE_ROW) {

		int x = sqlite3_column_int(enemy_pos_stmt, 2);
		int y = sqlite3_column_int(enemy_pos_stmt, 3);

		std::stringstream enemy_exec;
		enemy_exec << "SELECT * FROM Enemy WHERE id = " << sqlite3_column_int(enemy_pos_stmt, 1);
		sqlite3_stmt* enemy_stmt = nullptr;
		sqlite3_prepare_v2(db, enemy_exec.str().c_str(), -1, &enemy_stmt, nullptr);
		while (sqlite3_step(enemy_stmt) == SQLITE_ROW) {
			int id = sqlite3_column_int(enemy_stmt, 0);
			enemy* new_enemy = (enemy*)create_enemy(id, x, y, this);
			this->objects.push_back(new_enemy);
			this->enemies.push_back(new_enemy);
		}
		enemy_exec.clear();
		sqlite3_reset(enemy_stmt);
		sqlite3_finalize(enemy_stmt);
	}
	sqlite3_reset(enemy_pos_stmt);
	sqlite3_finalize(enemy_pos_stmt);

	sqlite3_close(db);
}

stage::~stage() {

}

void stage::enter_stage(int x, int y) {
	player::get_incetance()->enter_stage(this);
	player::get_incetance()->set_x(x);
	player::get_incetance()->set_y(y);
}

void stage::stack_obj(game_obj* obj) {
	this->objects.push_back(obj);
}

void stage::erase_obj(game_obj* obj) {
	int object_num = this->objects.size();
	for (int i = 0; i < object_num; i++) {
		if (obj == this->objects[i]) {
			this->objects.erase(objects.begin() + i);
			break;
		}
	}
	for (int i = 0; i < enemies.size(); i++) {
		if (obj == this->enemies[i]) {
			this->enemies.erase(enemies.begin() + i);
			break;
		}
	}
	delete obj;
}