#include "Quest.h"
#include <sstream>
#include <string>
#include "general.h"
#include "sqlite3.h"
#include "Application.h"
#include "Player.h"
#include "Enemy.h"

quest::quest(int id) {
	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	std::stringstream quest_exec;
	quest_exec << "SELECT * FROM Quest WHERE id = " << id;
	sqlite3_stmt* quest_stmt = nullptr;
	sqlite3_prepare(db, quest_exec.str().c_str(), -1, &quest_stmt, nullptr);
	if (sqlite3_step(quest_stmt) == SQLITE_ROW) {
		this->name = genmod::string_to_wstring((char*)sqlite3_column_text(quest_stmt, 1));
		this->description = genmod::string_to_wstring((char*)sqlite3_column_text(quest_stmt, 2));
		this->can_re_order = sqlite3_column_int(quest_stmt,3);
		this->reward_exp = sqlite3_column_int(quest_stmt, 4);
		this->reward_money = sqlite3_column_int(quest_stmt, 5);

		std::stringstream item_exec;
		item_exec << "SELECT * FROM Quest_item_reward WHERE quest_id = " << id;
		sqlite3_stmt* item_stmt;
		sqlite3_prepare(db, item_exec.str().c_str(), -1, &item_stmt, nullptr);
		while (sqlite3_step(item_stmt) == SQLITE_ROW) {
			this->reward_items.push_back(new reward_item());
			this->reward_items.back()->item_id = sqlite3_column_int(item_stmt, 1);
			this->reward_items.back()->num = sqlite3_column_int(item_stmt, 2);
		}
		sqlite3_reset(item_stmt);
		sqlite3_finalize(item_stmt);

		std::stringstream target_exec;
		target_exec << "SELECT * FROM Quest_target WHERE quest_id = " << id;
		sqlite3_stmt* target_stmt;
		int a = sqlite3_prepare(db, target_exec.str().c_str(), -1, &target_stmt, nullptr);
		while (sqlite3_step(target_stmt) == SQLITE_ROW) {
			this->targets.push_back(new target());
			this->targets.back()->enemy_id = sqlite3_column_int(target_stmt, 1);
			this->targets.back()->num = sqlite3_column_int(target_stmt, 2);
		}
		sqlite3_reset(target_stmt);
		sqlite3_finalize(target_stmt);
	}
	sqlite3_reset(quest_stmt);
	sqlite3_finalize(quest_stmt);
	sqlite3_close(db);
}

quest::~quest() {
	for (int i = 0; i < this->targets.size(); i++) {
		delete this->targets[i];
	}
}

void quest::check_dead_enemy(enemy* dead_enemy) {
	int enemy_id = dead_enemy->get_id();
	for (int i = 0; i < this->targets.size(); i++) {
		if (this->targets[i]->enemy_id == enemy_id)
			this->targets[i]->num--;
	}
}

bool quest::is_achived() {
	for (int i = 0; i < this->targets.size(); i++) {
		if (this->targets[i]->num != 0)
			return false;
	}
	return true;
}

void quest::achive() {
	player::get_incetance()->set_exp(player::get_incetance()->get_exp() + this->reward_exp);
}