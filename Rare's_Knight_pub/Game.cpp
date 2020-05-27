#include "DirectXModule.h"
#include "WindowsModule.h"
#include "Application.h"
#include "Player.h"
#include "sqlite3.h"
#include "general.h"
#include "Stage.h"
#include "Title.h"
#include "Map.h"
#include "SkillTree.h"
#include "Sound.h"
#include "Quest.h"
#include <string>
#include <sstream>
#include <fstream>
#include <istream>
#include <cstdio>
#include <cctype>
#include <algorithm>

game::game() {
	this->stack_scene(new title());

	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	sqlite3_stmt* enemy_stmt = nullptr;
	sqlite3_prepare(db, "SELECT * FROM Enemy", -1, &enemy_stmt, nullptr);
	while (sqlite3_step(enemy_stmt) == SQLITE_ROW) {
		this->enemies.push_back(new enemy(sqlite3_column_int(enemy_stmt, 0)));
	}
}

game::~game() {
	player::delete_incetance();
	for (int i = 0; i < scenes.size(); i++)
		delete scenes[i];
	for (int i = 0; i < next_erase_scene.size(); i++)
		delete next_erase_scene[i];
	for (int i = 0; i < next_stack_scene.size(); i++)
		delete next_stack_scene[i];
	for (int i = 0; i < sounds.size(); i++)
		delete sounds[i];
	for (int i = 0; i < active_quests.size(); i++)
		delete active_quests[i];
	for (int i = 0; i < enemies.size(); i++)
		delete enemies[i];
	winmod::win_end();
	dxmod::dx_end();
}

void game::draw() {
	dxmod::d2d_start_draw();
	int size = scenes.size();
	for (int i = 0; i < size; i++)
		scenes[i]->draw();
	dxmod::d2d_end_draw();
}

void game::calculation() {
	
	//ÉVÅ[ÉìçÌèú
	for (int i = 0; i < this->next_erase_scene.size(); i++) {
		for (int j = 0; j < this->scenes.size(); j++) {
			if (this->scenes[j] == this->next_erase_scene[i]) {
				delete this->scenes[j];
				this->scenes.erase(this->scenes.begin() + j);
				break;
			}
		}
	}
	this->next_erase_scene.clear();

	//ÉVÅ[Éììoò^
	//stack
	for (int i = 0; i < this->next_stack_scene.size(); i++) {
		this->scenes.push_back(this->next_stack_scene[i]);
	}
	this->next_stack_scene.clear();

	//enqueue
	for (int i = 0; i < this->next_enqueue_scene.size(); i++) {
		this->scenes.insert(this->scenes.begin(), this->next_enqueue_scene[i]);
	}
	this->next_enqueue_scene.clear();

	winmod::reset_key_state();

	
	int size = this->scenes.size();
	for (int i = 0; i < size; i++) {
		if(!scenes[i]->is_locked())
			scenes[i]->calc();
	}

	size = this->active_quests.size();
	for (int i = 0; i < size; i++) {
		if (this->active_quests[i]->is_achived()) {
			this->active_quests[i]->achive();
			delete this->active_quests[i];
			this->active_quests.erase(this->active_quests.begin() + i);
			i--;
		}
	}

	for (sound* i : this->sounds) {
		i->play();
	}
}

std::vector<quest*> game::get_active_quests() {
	return this->active_quests;
}

void game::erase_scene(scene* erased_scene) {
	this->next_erase_scene.push_back(erased_scene);
}

void game::stack_scene(scene* s_scene) {
	this->next_stack_scene.push_back(s_scene);
}

void game::enqueue_scene(scene* e_scene) {
	this->next_enqueue_scene.push_back(e_scene);
}

void game::stack_talk_event(int event_id) {
	this->talks.push_back(event_id);
}

void game::set_sound(int sound_data_id) {
	for (sound* itr : this->sounds) {
		if (itr->same_sound(sound_data_id)) {
			return;
		}
	}
	this->sounds.push_back(new sound(sound_data_id));
}

void game::erase_sound(int sound_data_id) {
	for (int i = 0; i < this->sounds.size(); i++) {
		if (sound_data_id == this->sounds[i]->get_sound_data_id()) {
			delete this->sounds[i];
			this->sounds.erase(this->sounds.begin() + i);
		}
	}
}

void game::reset_sound() {
	for (sound* sound_itr : this->sounds) {
			delete sound_itr;
	}
	this->sounds.clear();
}

int game::get_talk_event() {
	if (this->talks.empty()) {
		return -1;
	}
	int result;
	result = this->talks[0];
	this->talks.erase(this->talks.begin());
	return result;
}

void game::save_data(int file_id) {
	sqlite3* db;
	sqlite3_open("save_datas.db", &db);
	std::stringstream save_data_exec;

	save_data_exec << "DELETE FROM Save_data WHERE id = " << file_id;
	int a = sqlite3_exec(db, save_data_exec.str().c_str(), 0, 0, nullptr);
	save_data_exec.str("");
	
	save_data_exec << "INSERT INTO Save_data VALUES(";
	save_data_exec << file_id << "," << player::get_incetance()->get_maxhp() << "," << player::get_incetance()->get_hp() << "," << player::get_incetance()->get_atk() << "," << player::get_incetance()->get_def() << "," << player::get_incetance()->get_spd() << "," << player::get_incetance()->get_exp() << ")";
	sqlite3_exec(db, save_data_exec.str().c_str(), 0, 0, nullptr);
	save_data_exec.str("");

	save_data_exec << "DELETE FROM Skill WHERE File_id = " << file_id;
	sqlite3_exec(db, save_data_exec.str().c_str(), 0, 0, nullptr);
	save_data_exec.str("");

	for (int i = 0; i < player::get_incetance()->get_skills().size(); i++) {
		save_data_exec << "INSERT INTO Skill VALUES(";
		save_data_exec << file_id << "," << i << "," << (int)(player::get_incetance()->get_skills()[i]->is_active()) << ")";
		sqlite3_exec(db,save_data_exec.str().c_str(), 0, 0, nullptr);
		save_data_exec.str("");
	}
	
	save_data_exec.clear();
	sqlite3_close(db);

}


void game::load_data(int file_id) {
	sqlite3* db;
	sqlite3_open("save_datas.db", &db);
	std::stringstream file_exec;
	file_exec << "SELECT * FROM Save_data WHERE id = " << file_id;
	sqlite3_stmt* file_stmt;
	sqlite3_prepare(db, file_exec.str().c_str(), -1, &file_stmt, nullptr);
	if (sqlite3_step(file_stmt) == SQLITE_ROW) {
		player::get_incetance()->set_maxhp(sqlite3_column_int(file_stmt, 1));
		player::get_incetance()->set_hp(sqlite3_column_int(file_stmt, 2));
		player::get_incetance()->set_atk(sqlite3_column_int(file_stmt, 3));
		player::get_incetance()->set_def(sqlite3_column_int(file_stmt, 4));
		player::get_incetance()->set_spd(sqlite3_column_int(file_stmt, 5));
		player::get_incetance()->set_exp(sqlite3_column_int(file_stmt, 6));
		
		player::get_incetance()->init_skill();
		std::stringstream skill_exec;
		skill_exec << "SELECT * FROM Save_data WHERE File_id" << file_id;
		sqlite3_stmt* skill_stmt = nullptr;
		sqlite3_prepare(db, skill_exec.str().c_str(), -1, &skill_stmt, nullptr);
		while (sqlite3_step(skill_stmt) == SQLITE_ROW) {
			if (sqlite3_column_int(skill_stmt, 2)) {
				player::get_incetance()->get_skills()[sqlite3_column_int(skill_stmt, 1)]->activate();
			}
		}
		sqlite3_reset(skill_stmt);
		sqlite3_finalize(skill_stmt);
		
	}
	sqlite3_reset(file_stmt);
	sqlite3_finalize(file_stmt);
	sqlite3_close(db);

}

bool game::check_save_data(int file_id) {
	sqlite3* db;
	sqlite3_open("Save_datas.db", &db);
	sqlite3_stmt* stmt = nullptr;
	std::stringstream exec;
	exec << "SELECT * FROM Save_data WHERE id = " << file_id;
	sqlite3_prepare(db, exec.str().c_str(), -1, &stmt, nullptr);
	exec.clear();
	int prepare_result = sqlite3_step(stmt);
	sqlite3_reset(stmt);
	sqlite3_finalize(stmt);
	sqlite3_close(db);
	if (prepare_result == SQLITE_ROW)
		return true;
	return false;
}

void game::load_game() {
	for (int i = 0; i < this->scenes.size(); i++) {
		this->erase_scene(this->scenes[i]);
	}
	this->stack_scene(new map());
}