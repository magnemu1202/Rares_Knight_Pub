#pragma once
#include "GameObj.h"
#include "Skill.h"
#include <vector>

class stage;
class game;
class item;

class player : public game_obj {
protected:
	static player* incetance;

	int max_hp;
	int hp;
	int atk;
	int def;
	int spd;
	int exp;

	std::vector<skill*> skills;
	std::vector<item*> items;
	int skill_max_hp;
	int skill_atk;
	int skill_def;
	int skill_spd;

	int vel_y;
	int vel_x;

	static const int height = 200;
	static const int weight = 100;

	int stay_bitmap;
	int walk_bitmap;
	int run_bitmap;
	int wepon_bitmap;
	int jump_bitmap;
	int atk_bitmap[3];
	int atk_wave_bitmap[3];

	int bitmap_num;
	int bitmap_change_count;
	int uncontroll_count;

	int atack_delay;
	int combo_time;
	int atack_faze;

	stage* stage_inf;

	int invincible_count_down;

	bool turn_left;

	void atack(int atack_delay, int combo_time);

	void change_state(player* state);

public:

	//セッター
	void set_maxhp(int val);
	void set_hp(int val);
	void set_atk(int val);
	void set_def(int val);
	void set_spd(int val);
	void set_exp(int val);

	void set_skill_maxhp(int val);
	void set_skill_atk(int val);
	void set_skill_def(int val);
	void set_skill_spd(int val);

	//ゲッター
	int get_maxhp();
	int get_hp();
	int get_atk();
	int get_def();
	int get_spd();
	int get_exp();

	std::vector<skill*> get_skills() { return this->skills; };
	std::vector<item*> get_items() { return this->items; };

	int get_skill_maxhp();
	int get_skill_atk();
	int get_skill_def();
	int get_skill_spd();

	player();
	player(player* src);
	~player();
	void init_skill();
	void init_skill_param();
	void enter_stage(stage* stage_inf);
	void exit_stage();
	virtual void calc();
	virtual void draw(int camera_x, int camera_y) {};
	void lands();
	void take_damage(int damage);
	static player* get_incetance();
	static void delete_incetance();
};

class stay_player : public player {
public:
	stay_player(player* src);
	void calc();
	void draw(int camera_x, int camera_y);
};

class walk_player : public player {
public:
	walk_player(player* src);
	void calc();
	void draw(int camera_x, int camera_y);
};

class run_player : public player {
public:
	run_player(player* src);
	void calc();
	void draw(int camera_x, int camera_y);
};

class jump_player : public player {
public:
	jump_player(player* src);
	void calc();
	void draw(int camera_x, int camera_y);
};

class atk0_player : public player {
public:
	atk0_player(player* src);
	void calc();
	void draw(int camera_x, int camera_y);
};

class atk1_player : public player {
public:
	atk1_player(player* src);
	void calc();
	void draw(int camera_x, int camera_y);
};

class atk2_player : public player {
public:
	atk2_player(player* src);
	void calc();
	void draw(int camera_x, int camera_y);
};