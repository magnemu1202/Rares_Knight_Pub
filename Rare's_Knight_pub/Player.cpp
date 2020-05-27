#include "DirectXModule.h"
#include "WindowsModule.h"
#include "Application.h"
#include "general.h"
#include "Player.h"
#include "Stage.h"
#include "Item.h"
#include <sstream>
#include <string>
#include <cstring>
#include "sqlite3.h"
#include "ImageManager.h"

player* player::incetance = new player();

void player::set_maxhp(int val) { this->max_hp = val; }
void player::set_hp(int val) {
	this->hp = val;
	if (this->max_hp < this->hp)
		this->hp = this->max_hp;
}
void player::set_atk(int val) { this->atk = val; }
void player::set_def(int val) { this->def = val; }
void player::set_spd(int val) { this->spd = val; }
void player::set_exp(int val) { this->exp = val; }

void player::set_skill_maxhp(int val) { this->skill_max_hp = val; }
void player::set_skill_atk(int val) { this->skill_atk = val; }
void player::set_skill_def(int val) { this->skill_def = val; }
void player::set_skill_spd(int val) { this->skill_spd = val; }

int player::get_maxhp() { return this->max_hp; };
int player::get_hp() { return this->hp; };
int player::get_atk() { return this->atk; };
int player::get_def() { return this->def; };
int player::get_spd() { return this->spd; };
int player::get_exp() { return this->exp; };

int player::get_skill_maxhp() { return this->skill_max_hp; }
int player::get_skill_atk() { return this->skill_atk; }
int player::get_skill_def() { return this->skill_def; }
int player::get_skill_spd() { return this->skill_spd; }

#define SQUARE_OF_PIXCEL 100

player::player() {
	this->turn_left = true;
	this->bitmap_num = 0;
	this->bitmap_change_count;
	this->vel_y = 0;
	this->invincible_count_down = 0;
	this->atack_faze = 0;

	this->max_hp = this->hp = 30;
	this->atk = 1;
	this->def = 1;
	this->spd = 0;

	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	sqlite3_stmt* skill_stmt = nullptr;
	sqlite3_prepare(db, "SELECT * FROM Skill", -1, &skill_stmt, nullptr);
	while (sqlite3_step(skill_stmt) == SQLITE_ROW) {
		this->skills.push_back(new skill(sqlite3_column_int(skill_stmt, 0), this));
	}
	sqlite3_reset(skill_stmt);
	sqlite3_finalize(skill_stmt);
	
	sqlite3_stmt* item_stmt = nullptr;
	sqlite3_prepare(db, "SELECT * FROM Item", -1, &item_stmt, nullptr);
	while (sqlite3_step(item_stmt) == SQLITE_ROW) {
		this->items.push_back(create_item(sqlite3_column_int(item_stmt, 0)));
	}
	this->items[0]->set_num(1);
	sqlite3_reset(item_stmt);
	sqlite3_finalize(item_stmt);

	sqlite3_close(db);
}

player::player(player* src) {
	this->x = src->x;
	this->y = src->y;
	this->vel_x = src->vel_x;
	this->vel_y = src->vel_y;
	this->hp = src->hp;
	this->max_hp = src->max_hp;
	this->atk = src->atk;
	this->def = src->def;
	this->spd = src->spd;
	this->exp = src->exp;
	this->skills = src->skills;
	this->items = src->items;
	this->skill_max_hp = src->skill_max_hp;
	this->skill_atk = src->skill_atk;
	this->skill_def = src->skill_def;
	this->skill_spd = src->skill_spd;
	this->wepon_bitmap = src->wepon_bitmap;
	this->stay_bitmap = src->stay_bitmap;
	this->walk_bitmap = src->walk_bitmap;
	this->run_bitmap = src->run_bitmap;
	this->jump_bitmap = src->jump_bitmap;
	this->atk_bitmap[0] = src->atk_bitmap[0];
	this->atk_bitmap[1] = src->atk_bitmap[1];
	this->atk_bitmap[2] = src->atk_bitmap[2];
	this->atk_wave_bitmap[0] = src->atk_wave_bitmap[0];
	this->atk_wave_bitmap[1] = src->atk_wave_bitmap[1];
	this->atk_wave_bitmap[2] = src->atk_wave_bitmap[2];
	this->atack_delay = src->atack_delay;
	this->combo_time = src->combo_time;
	this->uncontroll_count = src->uncontroll_count;
	this->stage_inf = src->stage_inf;
	this->invincible_count_down = src->invincible_count_down;
	this->turn_left = src->turn_left;
}

player::~player() {
	for (int i = 0; i < this->skills.size(); i++) {
		delete this->skills[i];
	}
	this->skills.clear();
	for (int i = 0; i < this->items.size(); i++) {
		delete this->items[i];
	}
	this->items.clear();
	image_manager::release_image(this->stay_bitmap);
	image_manager::release_image(this->run_bitmap);
	image_manager::release_image(this->walk_bitmap);
	image_manager::release_image(this->jump_bitmap);
	image_manager::release_image(this->wepon_bitmap);
	image_manager::release_image(this->atk_bitmap[0]);
	image_manager::release_image(this->atk_bitmap[1]);
	image_manager::release_image(this->atk_bitmap[2]);
}

void player::enter_stage(stage* stage_inf) {
	if (this->stage_inf == nullptr) {
		this->stay_bitmap = image_manager::get_image_id(L"image/stay.png");
		this->run_bitmap = image_manager::get_image_id(L"image/run.png");
		this->walk_bitmap = image_manager::get_image_id(L"image/walk.png");
		this->jump_bitmap = image_manager::get_image_id(L"image/jump.png");
		this->wepon_bitmap = image_manager::get_image_id(L"image/wepon0.png");
		this->atk_bitmap[0] = image_manager::get_image_id(L"image/atk0.png");
		this->atk_bitmap[1] = image_manager::get_image_id(L"image/atk1.png");
		this->atk_bitmap[2] = image_manager::get_image_id(L"image/atk2.png");
		this->atk_wave_bitmap[0] = image_manager::get_image_id(L"image/atk_wave0.png");
		this->atk_wave_bitmap[1] = image_manager::get_image_id(L"image/atk_wave1.png");
		this->atk_wave_bitmap[2] = image_manager::get_image_id(L"image/atk_wave2.png");
	}
	this->stage_inf = stage_inf;
	this->atack_delay = 0;
	this->combo_time = 0;
	this->change_state(new stay_player(this));
}

void player::exit_stage() {
	this->stage_inf = nullptr;
	image_manager::release_image(this->stay_bitmap);
	image_manager::release_image(this->walk_bitmap);
	image_manager::release_image(this->run_bitmap);
	image_manager::release_image(this->jump_bitmap);
	image_manager::release_image(this->jump_bitmap);
	image_manager::release_image(this->atk_bitmap[0]);
	image_manager::release_image(this->atk_bitmap[1]);
	image_manager::release_image(this->atk_bitmap[2]);
	image_manager::release_image(this->atk_wave_bitmap[0]);
	image_manager::release_image(this->atk_wave_bitmap[1]);
	image_manager::release_image(this->atk_wave_bitmap[2]);
}

void player::init_skill() {
	this->skill_max_hp = 0;
	this->skill_atk = 0;
	this->skill_def = 0;
	this->skill_spd = 0;
	for (int i = 0; i < this->skills.size(); i++) {
		this->skills[i]->init();
	}
}

void player::init_skill_param() {
	for (int i = 0; i < this->skills.size(); i++) {
		this->skills[i]->init_skill_param();
	}
}

void player::change_state(player* state) {
	//thisの参照を意味のないものにし、デストラクタでリソースが破棄されないようにする。
	this->wepon_bitmap = -1;
	this->stay_bitmap = -1;
	this->walk_bitmap = -1;
	this->run_bitmap = -1;
	this->jump_bitmap = -1;
	this->atk_bitmap[0] = -1;
	this->atk_bitmap[1] = -1;
	this->atk_bitmap[2] = -1;
	this->atk_wave_bitmap[0] = -1;
	this->atk_wave_bitmap[1] = -1;
	this->atk_wave_bitmap[2] = -1;
	std::vector<skill*> tmp_skills;
	this->skills = tmp_skills;
	std::vector<item*> tmp_items;
	this->items = tmp_items;
	
	//インスタンス破棄
	delete incetance;

	//インスタンス代入
	incetance = state;
}

void player::calc() {
	//無敵カウントダウン
	this->invincible_count_down--;
	if (this->invincible_count_down <= 0) {
		this->invincible_count_down = 0;
	}

	//画像切り替わりカウント
	if (this->bitmap_change_count++ > 6) {
		this->bitmap_change_count = 0;
		this->bitmap_num = (this->bitmap_num + 1) % 3;
	}

	//操作不能カウント
	if (--this->uncontroll_count < 0) {
		this->uncontroll_count = 0;
	}

	if (--this->atack_delay < 0) {
		this->atack_delay = 0;
	}
	if (--this->combo_time < 0) {
		this->combo_time = 0;
	}

	if (winmod::get_key_down_state(VK_LEFT)) {
		this->turn_left = false;
	}
	else if (winmod::get_key_down_state(VK_RIGHT)) {
		this->turn_left = true;
	}

	this->vel_y++;
	this->y += vel_y;
	this->x += vel_x;
	this->lands();

	this->stage_inf->set_camera_pos(this->x,this->y);
}

void player::lands() {
	char** map_data = stage_inf->get_mapdata();

	//坂道にいる
	if (map_data[this->y / 100][this->x / 100] == 3 && (this->y / 100) * 100 - (this->x % 100) <= this->y) {
		this->y = (this->y / 100) * 100 - (this->x % 100);
		this->vel_y = 0;
	}
	else if (map_data[(this->y + 100) / 100][this->x / 100] == 3 && ((this->y + 100) / 100) * 100 - (this->x % 100) <= this->y) {
		this->y = ((this->y + 100) / 100) * 100 - (this->x % 100);
		this->vel_y = 0;
	}
	else if (map_data[this->y / 100][this->x / 100] == 4 && (this->y / 100) * 100 - 100 + (this->x % 100) <= this->y) {
		this->y = (this->y / 100) * 100- 100+ (this->x % 100);
		this->vel_y = 0;
	}
	else if (map_data[(this->y + 100) / 100][this->x / 100] == 4 && ((this->y + 100) / 100) * 100 - 100 + (this->x % 100) <= this->y) {
		this->y = ((this->y + 100) / 100) * 100 - 100 + (this->x % 100);
		this->vel_y = 0;
	}
	else {
		//左にぶつかる
		if (map_data[this->y / 100][(this->x - this->weight/2) / 100] == 1 ) {
			this->x = ((this->x - this->weight/2) / 100) * 100 + 100 + this->weight/2;
		}
		//右にぶつかる
		else if (map_data[this->y / 100][(this->x + this->weight/2) / 100] == 1) {
			this->x = ((this->x - this->weight/2) / 100) * 100 + this->weight/2;
		}
		//下にぶつかる
		if (map_data[(this->y + this->height/2) / 100][this->x / 100] == 1) {
			this->y = ((this->y + this->height/2) / 100) * 100 - this->height/2;
			this->vel_y = 0;
		}
		//上にぶつかる
		else if (map_data[(this->y - 50) / 100][this->x / 100] == 1 || map_data[(this->y - 50) / 100][this->x / 100] == 3 || map_data[(this->y - 50) / 100][this->x / 100] == 4) {
			this->y = ((this->y+ 50) / 100) * 100 + 50;
			this->vel_y = -this->vel_y;
		}
	}
	//マップ端
	if (this->x < 50)
		this->x = 50;
	else if (this->x > (this->stage_inf->get_column_num() * 100) - 50)
		this->x = this->stage_inf->get_column_num() * 100 - 50;
}

void player::atack(int atack_delay,int combo_time) {
	this->atack_delay = atack_delay;
	this->combo_time = combo_time;
	for (int i = 0; i < this->stage_inf->get_enemy().size(); i++) {
		int target_x = this->stage_inf->get_enemy()[i]->get_x();
		int target_y = this->stage_inf->get_enemy()[i]->get_y();

		if (!turn_left && std::abs(this->x - target_x - 125) < 125 && std::abs(this->y - target_y) < 200) {
			(this->stage_inf->get_enemy()[i])->damage(this->atk + this->skill_atk,true);
		}
		else if (turn_left && std::abs(this->x - target_x + 125) < 125 && std::abs(this->y - target_y) < 100) {
			(this->stage_inf->get_enemy()[i])->damage(this->atk + this->skill_atk,true);
		}
	}
}

player* player::get_incetance() {
	return incetance;
}

void player::delete_incetance() {
	delete incetance;
}

void player::take_damage(int damage) {
	if (this->invincible_count_down == 0) {
		this->uncontroll_count = 40;
		this->vel_y = -20;
		this->invincible_count_down = 120;
		if (damage > this->def+this->skill_def)
			this->hp -= damage - this->def + skill_def;
		else
			this->hp--;
	}
}

stay_player::stay_player(player* src) : player(src) {

}

void stay_player::calc() {
	this->vel_x /= 2;
	player::calc();
	if (winmod::get_key_down_state('Z')) {
		this->atack(15, 30);
		this->change_state(new atk0_player(this));
	}
	else if (winmod::get_key_down_state(VK_UP)) {
		this->vel_y = -20;
		this->change_state(new jump_player(this));
	}
	else if (winmod::get_key_state(VK_RIGHT) || winmod::get_key_state(VK_LEFT)) {
		if (winmod::get_key_state(VK_SHIFT)) {
			this->change_state(new run_player(this));
		}
		else {
			this->change_state(new walk_player(this));
		}
	}
}

void stay_player::draw(int camera_x,int camera_y) {
	if (!(invincible_count_down % 2)) {
		if (turn_left) {
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 0, 0);
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->stay_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, this->bitmap_num, 0);
		}
		else {
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 0, 0);
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->stay_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, this->bitmap_num, 0);
		}
	}
}

walk_player::walk_player(player* src) : player(src) {

}

void walk_player::calc() {
	player::calc();
	if (this->turn_left) {
		this->vel_x = 3;
	}
	else {
		this->vel_x = -3;
	}

	if (winmod::get_key_down_state('Z')) {
		this->atack(15, 30);
		this->change_state(new atk0_player(this));
	}
	else if (winmod::get_key_down_state(VK_UP)) {
		this->vel_y = -20;
		this->change_state(new jump_player(this));
	}
	else if (!(winmod::get_key_state(VK_RIGHT) || winmod::get_key_state(VK_LEFT))) {
		this->change_state(new stay_player(this));
	}
	else if (winmod::get_key_state(VK_SHIFT)) {
		this->change_state(new run_player(this));
	}
}

void walk_player::draw(int camera_x, int camera_y) {
	if (!(invincible_count_down % 2)) {
		if (turn_left) {
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 0, 0);
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->walk_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, this->bitmap_num, 0);
		}
		else {
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 0, 0);
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->walk_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, this->bitmap_num, 0);
		}
	}
}

run_player::run_player(player* src) : player(src) {

}

void run_player::calc() {
	player::calc();
	if (this->turn_left) {
		this->vel_x = 6;
	}
	else {
		this->vel_x = -6;
	}
	if (winmod::get_key_down_state('Z')) {
		this->atack(15, 30);
		this->change_state(new atk0_player(this));
	}
	else if (winmod::get_key_down_state(VK_UP)) {
		this->vel_y = -20;
		this->change_state(new jump_player(this));
	}
	else if (!(winmod::get_key_state(VK_SHIFT)) || (!winmod::get_key_state(VK_RIGHT) && !winmod::get_key_state(VK_LEFT))) {
		if (!(winmod::get_key_state(VK_SHIFT))) {
			this->change_state(new walk_player(this));
		}
		else {
			this->change_state(new stay_player(this));
		}
	}
}

void run_player::draw(int camera_x, int camera_y) {
	if (!(invincible_count_down % 2)) {
		if (turn_left) {
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 0, 0);
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->run_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, this->bitmap_num, 0);
		}
		else {
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 0, 0);
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->run_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, this->bitmap_num, 0);
		}
	}
}

jump_player::jump_player(player* src) : player(src) {

}

void jump_player::calc() {
	player::calc();
	if (winmod::get_key_down_state('Z')) {
		this->atack(15, 30);
		this->change_state(new atk0_player(this));
	}
	else if (stage_inf->get_mapdata()[(this->y + this->height / 2) / 100][this->x / 100] == 1 && this->vel_y == 0) {
		if (!(winmod::get_key_down_state(VK_RIGHT) || winmod::get_key_down_state(VK_LEFT))) {
			this->change_state(new stay_player(this));
		}
		else if (winmod::get_key_down_state(VK_SHIFT)) {
			this->change_state(new run_player(this));
		}
		else {
			this->change_state(new walk_player(this));
		}
	}
}

void jump_player::draw(int camera_x, int camera_y) {
	if (!(invincible_count_down % 2)) {
		if (turn_left) {
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 0, 0);
			dxmod::d2d_draw_bitmap(this->jump_bitmap, this->x - camera_x, this->y - camera_y);
		}
		else {
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 0, 0);
			dxmod::d2d_draw_xreversed_bitmap(this->jump_bitmap, this->x - camera_x, this->y - camera_y);
		}
	}
}

atk0_player::atk0_player(player* src) : player(src) {

}

void atk0_player::calc() {
	player::calc();
	if (this->vel_x > 0) {
		this->vel_x--;
	}
	else if(this->vel_x < 0){
		this->vel_x++;
	}
	if (winmod::get_key_down_state('Z') && this->atack_delay == 0) {
		this->atack(15, 30);
		this->change_state(new atk1_player(this));
	}
	else if (this->combo_time == 0) {
		if (!(winmod::get_key_down_state(VK_RIGHT) || winmod::get_key_down_state(VK_LEFT))) {
			this->change_state(new stay_player(this));
		}
		else if (winmod::get_key_down_state(VK_SHIFT)) {
			this->change_state(new run_player(this));
		}
		else {
			this->change_state(new walk_player(this));
		}
	}
}

void atk0_player::draw(int camera_x, int camera_y) {
	if (!(invincible_count_down % 2)) {
		if (turn_left) {
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 1, 0);
			dxmod::d2d_draw_bitmap(this->atk_bitmap[0], this->x - camera_x, this->y - camera_y);
		}
		else {
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 1, 0);
			dxmod::d2d_draw_xreversed_bitmap(this->atk_bitmap[0], this->x - camera_x, this->y - camera_y);
		}
	}
	if (this->atack_delay > 0) {
		if (this->turn_left)
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->atk_wave_bitmap[0], this->x - camera_x + 50, this->y - camera_y - 100, 200, 200, 1 - (this->atack_delay / 7), 0);
		else
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->atk_wave_bitmap[0], this->x - camera_x - 200, this->y - camera_y - 100, 200, 200, 1 - (this->atack_delay / 7), 0);
	}
}

atk1_player::atk1_player(player* src) : player(src) {

}

void atk1_player::calc() {
	player::calc();
	if (winmod::get_key_down_state('Z') && this->atack_delay == 0) {
		this->atack(15, 30);
		this->change_state(new atk2_player(this));
	}
	else if (this->combo_time == 0) {
		if (!(winmod::get_key_down_state(VK_RIGHT) || winmod::get_key_down_state(VK_LEFT))) {
			this->change_state(new stay_player(this));
		}
		else if (winmod::get_key_down_state(VK_SHIFT)) {
			this->change_state(new run_player(this));
		}
		else {
			this->change_state(new walk_player(this));
		}
	}
}

void atk1_player::draw(int camera_x, int camera_y) {
	if (!(invincible_count_down % 2)) {
		if (turn_left) {
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 2, 0);
			dxmod::d2d_draw_bitmap(this->atk_bitmap[1], this->x - camera_x, this->y - camera_y);
		}
		else {
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 2, 0);
			dxmod::d2d_draw_xreversed_bitmap(this->atk_bitmap[1], this->x - camera_x, this->y - camera_y);
		}
	}
	if (this->atack_delay > 0) {
		if (this->turn_left)
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->atk_wave_bitmap[1], this->x - camera_x + 50, this->y - camera_y - 100, 200, 200, 1 - (this->atack_delay / 7), 0);
		else
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->atk_wave_bitmap[1], this->x - camera_x - 200, this->y - camera_y - 100, 200, 200, 1 - (this->atack_delay / 7), 0);
	}
}

atk2_player::atk2_player(player* src) : player(src) {

}

void atk2_player::calc() {
	player::calc();
	if (this->combo_time == 0) {
		if (!(winmod::get_key_down_state(VK_RIGHT) || winmod::get_key_down_state(VK_LEFT))) {
			this->change_state(new stay_player(this));
		}
		else if (winmod::get_key_down_state(VK_SHIFT)) {
			this->change_state(new run_player(this));
		}
		else {
			this->change_state(new walk_player(this));
		}
	}
}

void atk2_player::draw(int camera_x, int camera_y) {
	if (!(invincible_count_down % 2)) {
		if (turn_left) {
			dxmod::d2d_draw_bitmap(this->atk_bitmap[2], this->x - camera_x, this->y - camera_y);
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 3, 0);
		}
		else {
			dxmod::d2d_draw_xreversed_bitmap(this->atk_bitmap[2], this->x - camera_x, this->y - camera_y);
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->wepon_bitmap, this->x - camera_x - 100, this->y - camera_y - 100, 200, 200, 3, 0);
		}
	}
	if (this->atack_delay > 0) {
		if (this->turn_left)
			dxmod::d2d_draw_part_of_bitmap_NumUL(this->atk_wave_bitmap[2], this->x - camera_x + 50, this->y - camera_y - 100, 200, 200, 1 - (this->atack_delay / 7), 0);
		else
			dxmod::d2d_draw_part_of_xreversed_bitmap_NumUL(this->atk_wave_bitmap[2], this->x - camera_x - 200, this->y - camera_y - 100, 200, 200, 1 - (this->atack_delay / 7), 0);
	}
}