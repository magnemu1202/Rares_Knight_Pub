#include "Skill.h"
#include <sstream>
#include "general.h"
#include "Player.h"
#include "sqlite3.h"

skill::skill(int id, player* m_player) {
	this->id = id;
	this->m_player = m_player;

	//ƒXƒLƒ‹“Ç‚İ‚İ
	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	sqlite3_stmt* skill_stmt = nullptr;
	std::stringstream skill_exec;
	skill_exec << "SELECT * FROM Skill WHERE id = " << id;
	int a = sqlite3_prepare(db, skill_exec.str().c_str(), -1, &skill_stmt, nullptr);
	while (sqlite3_step(skill_stmt) == SQLITE_ROW) {
		this->main_text = genmod::string_to_wstring((char*)sqlite3_column_text(skill_stmt, 1));
		this->sub_text = genmod::string_to_wstring((char*)sqlite3_column_text(skill_stmt, 2));
		this->p_hp = sqlite3_column_int(skill_stmt, 4);
		this->p_atk = sqlite3_column_int(skill_stmt, 5);
		this->p_def = sqlite3_column_int(skill_stmt, 6);
		this->p_spd = sqlite3_column_int(skill_stmt, 7);
		std::stringstream skill_node_exec;
		skill_node_exec << "SELECT * FROM Skill_tree WHERE origin_Skill_id = " << id;
		sqlite3_stmt* skill_node_stmt = nullptr;
		sqlite3_prepare(db, skill_node_exec.str().c_str(), -1, &skill_node_stmt, nullptr);
		while (sqlite3_step(skill_node_stmt) == SQLITE_ROW) {
			node* new_node = new node();
			new_node->origin = sqlite3_column_int(skill_node_stmt, 0);
			new_node->next = sqlite3_column_int(skill_node_stmt, 1);
			this->nodes.push_back(new_node);
		}
		sqlite3_reset(skill_node_stmt);
		sqlite3_finalize(skill_node_stmt);
	}
	skill_exec.clear();
	sqlite3_reset(skill_stmt);
	sqlite3_finalize(skill_stmt);
	sqlite3_close(db);

	this->active = false;
}

skill::~skill() {
	for (int i = 0; i < this->nodes.size(); i++) {
		delete this->nodes[i];
	}
	this->nodes.clear();
}

void skill::init() {
	this->active = false;
}

void skill::activate() {
	this->active = true;
}

void skill::init_skill_param() {
	player::get_incetance()->set_skill_maxhp(player::get_incetance()->get_skill_maxhp() + this->p_hp);
	player::get_incetance()->set_skill_atk(player::get_incetance()->get_skill_atk() + this->p_atk);
	player::get_incetance()->set_skill_def(player::get_incetance()->get_skill_def() + this->p_def);
	player::get_incetance()->set_skill_spd(player::get_incetance()->get_skill_spd() + this->p_spd);
}

bool skill::is_active() {
	return this->active;
}