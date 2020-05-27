#include "DirectXModule.h"
#include "WindowsModule.h"
#include "general.h"
#include "Application.h"
#include "Enemy.h"
#include "Player.h"
#include "Stage.h"
#include "Quest.h"
#include "sqlite3.h"
#include <sstream>
#include "ImageManager.h"

game_obj* create_enemy(int enemy_id, int x,int y,stage* stage_inf) {
	if (enemy_id == 0) {
		return new slime(x,y,stage_inf);
	}
}

enemy::enemy(int id) {
	this->id = id;
	
	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	std::stringstream enemy_exec;
	enemy_exec << "SELECT * FROM Enemy WHERE id = " << id;
	sqlite3_stmt* enemy_stmt;
	sqlite3_prepare(db, enemy_exec.str().c_str(), -1, &enemy_stmt, nullptr);
	if (sqlite3_step(enemy_stmt) == SQLITE_ROW) {
		this->name = genmod::string_to_wstring((char*)sqlite3_column_text(enemy_stmt, 1));
		this->max_hp = this->hp = sqlite3_column_int(enemy_stmt, 3);
		this->atk = sqlite3_column_int(enemy_stmt, 4);
		this->def = sqlite3_column_int(enemy_stmt, 5);
		this->drop_exp = sqlite3_column_int(enemy_stmt, 6);
	}
}

int enemy::get_id() {
	return this->id;
}

void enemy::lands() {
	char** map_data = stage_inf->get_mapdata();

	//坂道にいる
	if (map_data[(this->y + (this->height/2)) / 100][this->x / 100] == 3 && this->x % 100 > (this->y + 50) % 100) {
		this->y = (this->y / 100) * 100 + (this->x % 100);
	}
	else if (map_data[(this->y + 50) / 100][this->x / 100] == 4 &&  100 + (this->x % 100) + ((this->y + 50) % 100) < 0) {
		this->y = (this->y / 100) * 100 + (this->x % 100);
	}
	else {
		//左にぶつかる
		if (map_data[this->y / 100][(this->x - (this->weidth/2)) / 100] == 1) {
			this->x = ((this->x - this->weidth/2) / 100) * 100 + this->weidth/2;
		}
		//右にぶつかる
		else if (map_data[this->y / 100][(this->x + (this->weidth/2)) / 100] == 1) {
			this->x = ((this->x + this->weidth/2) / 100) * 100 - this->weidth/2;
		}
		//下にぶつかる
		if (map_data[(this->y + this->height/2) / 100][this->x / 100] == 1) {
			this->y = ((this->y + this->height/2) / 100) * 100 - this->height/2;
			this->vel_y = 0;
		}
		//上にぶつかる
		else if (map_data[(this->y - 50) / 100][this->x / 100] == 1) {
			this->y = ((this->y - 50) / 100) * 100 + 50;
			this->vel_y = -this->vel_y;
		}
	}

	if (this->x < 50) {
		this->x = 50;
	}
	else if (this->x > (this->stage_inf->get_column_num() * 100) - 50)
		this->x = this->stage_inf->get_column_num() * 100 - 50;
}

void enemy::damage(int atk,bool nock_back) {
	if (atk - this->def < 0) {
		this->hp--;
	}
	else {
		this->hp -= atk - this->def;
	}
	if (nock_back) {
		if (this->x < player::get_incetance()->get_x()) {
			this->turn_left = false;
		}
		else {
			this->turn_left = true;
		}
		this->vel_y = -20;
		this->act = NOCK_BACK;
	}
	else {
		this->act = NO_ACTION;
	}
	//死亡判定
	if (this->hp < 0) {
		std::vector<quest*> active_quests = game::get_incetance().get_active_quests();
		for (int i = 0; i < active_quests.size(); i++) {
			active_quests[i]->check_dead_enemy(this);
		}
		player::get_incetance()->set_exp(player::get_incetance()->get_exp() + this->drop_exp);
		this->stage_inf->erase_obj(this);
	}
}

slime::slime(int x, int y,stage* stage_inf) : enemy(0){
	
	this->bitmap = image_manager::get_image_id(L"image/slime.png");
	
	this->drop_item = drop_item;
	this->x = x;
	this->y = y;
	this->bitmap_num = 0;
	this->act_change_count = 0;
	this->invincible_count = 0;
	this->change_bitmap_count = 0;
	this->act = NO_ACTION;
	this->stage_inf = stage_inf;
	this->vel_y = 0;
	this->vel_x = 0;
	this->weidth = 100;
	this->height = 100;
}

slime::~slime() {
	image_manager::release_image(this->bitmap);
}

void slime::calc() {
	if (--this->change_bitmap_count < 0 && (this->act == ATACK || this->act == LEFT_WALK || this->act == RIGHT_WALK)) {
		this->change_bitmap_count = 10;
		this->bitmap_num = (this->bitmap_num + 1) % 2;
	}

	//ユークリッド距離で判定
	bool close_pos = (player::get_incetance()->get_x() - this->x) * (player::get_incetance()->get_x() - this->x) + (player::get_incetance()->get_y() - this->y) * (player::get_incetance()->get_y() - this->y) < 16000;
	
	
	/*状態遷移*/
	if (this->act == NOCK_BACK) {
		if (++this->act_change_count > 100)
			this->act = NO_ACTION;
	}
	else if (close_pos){
		this->act = ATACK;
	}
	else if(this->act == ATACK && !close_pos) {
		this->act = NO_ACTION;
	}
	else if(++this->act_change_count > 100){
		this->act_change_count = 0;
		int rand_num = winmod::get_rand();
		if (rand_num % 3 == 0) {
			this->act = NO_ACTION;
		}
		else if (rand_num % 3 == 1) {
			this->act = LEFT_WALK;
		}
		else {
			this->act = RIGHT_WALK;
		}
	}

	/*与ダメージ判定*/
	if ((player::get_incetance()->get_x() - this->x) * (player::get_incetance()->get_x() - this->x) + (player::get_incetance()->get_y() - this->y) * (player::get_incetance()->get_y() - this->y) < 5000) {
		player::get_incetance()->take_damage(this->atk);
	}


	/*行動*/

	this->vel_y++;
	this->y += vel_y;
	this->lands();

	if (this->act == NOCK_BACK) {
		if (this->turn_left) {
			this->vel_x = 2;
		}
		else {
			this->vel_x = -2;
		}
	}
	else if (this->act == ATACK) {
		if (player::get_incetance()->get_x() < this->x) {
			this->turn_left = true;
			this->vel_x = -1;
		}
		else {
			this->turn_left = false;
			this->vel_x = 1;
		}
	}
	else if (this->act == RIGHT_WALK) {
		this->turn_left = false;
		this->vel_x = 1;
	}
	else if (this->act == LEFT_WALK) {
		this->turn_left = true;
		this->vel_x = -1;
	}
	else
		this->vel_x = 0;
	this->x += vel_x;
}

void slime::draw(int camera_x,int camera_y) {
	if (!this->invincible_count % 2) {
		if (turn_left)
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->bitmap, this->x - camera_x - this->weidth / 2, this->y - camera_y - this->height / 2, 100, 100, bitmap_num, 0);
		else
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->bitmap, this->x - camera_x - this->weidth / 2, this->y - camera_y - this->height / 2, 100, 100, bitmap_num, 0);
	}
}