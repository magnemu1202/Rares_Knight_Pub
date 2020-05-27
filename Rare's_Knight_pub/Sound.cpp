#include "Sound.h"
#include <sstream>
#include "DirectXModule.h"
#include "Application.h"
#include "sqlite3.h"

sound::sound(int sound_data_id) {
	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	sqlite3_stmt* sound_stmt = nullptr;
	std::stringstream sound_exec;
	sound_exec << "SELECT * FROM Sound Where id = " << sound_data_id;
	sqlite3_prepare(db, sound_exec.str().c_str(), -1, &sound_stmt, nullptr);
	sqlite3_step(sound_stmt);
	this->is_play = false;
	this->sound_data_id = sound_data_id;
	this->sound_id = dxmod::ds_load_wave_from_file((char*)sqlite3_column_text(sound_stmt, 1));
	this->resume = sqlite3_column_int(sound_stmt, 2);
	this->resume_point = sqlite3_column_int(sound_stmt, 3);
	this->return_point = sqlite3_column_int(sound_stmt, 4);
	this->volume = sqlite3_column_int(sound_stmt, 5);
	dxmod::ds_set_volume(this->sound_id,this->volume);
	sqlite3_reset(sound_stmt);
	sqlite3_finalize(sound_stmt);
	sqlite3_close(db);
}

sound::~sound() {
	dxmod::ds_release_wave(this->sound_id);
}

void sound::play() {
	
	if (dxmod::ds_get_current_cursor(this->sound_id) > 0) {
		this->is_play = true;
	}

	if (!this->is_play) {
		dxmod::ds_sound_play(this->sound_id);
		dxmod::ds_set_volume(this->sound_id, this->volume);
	}
	else if (dxmod::ds_get_current_cursor(this->sound_id) == 0) {
		game::get_incetance().erase_sound(this->sound_data_id);
	}
	else if (this->resume && this->resume_point < dxmod::ds_get_current_cursor(this->sound_id)) {
		dxmod::ds_sound_set_pos(this->sound_id,this->return_point);
	}
}

void sound::stop() {
	dxmod::ds_sound_stop(this->sound_id);
}

bool sound::same_sound(int sound_data_id) {
	bool same = sound_data_id == this->sound_data_id;
	return same;
}

int sound::get_sound_data_id() {
	return this->sound_data_id;
}