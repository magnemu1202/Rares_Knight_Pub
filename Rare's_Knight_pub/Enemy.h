#pragma once
#include "GameObj.h"
#include <vector>

class item;
class stage;
class game;

#define SQUARE_OF_PIXCEL 100

enum action {
	NO_ACTION,
	RIGHT_WALK,
	LEFT_WALK,
	ATACK,
	NOCK_BACK
};

class enemy : public game_obj {
protected:
	int id;
	int max_hp;
	int hp;
	int atk;
	int def;

	int vel_y;
	int vel_x;

	std::wstring name;

	int weidth;
	int height;

	bool turn_left;
	int change_bitmap_count;
	int invincible_count;

	int drop_exp;
	std::vector<item*> drop_item;

	;
	stage* stage_inf;

	action act;

public:
	enemy(int id);
	virtual ~enemy() {};
	int get_id();
	std::wstring get_name() { return this->name; };
	void lands();
	virtual void calc() {};
	virtual void draw(int camera_x, int camera_y) {};
	void damage(int atk,bool nock_back);

};

class slime : public enemy {
private:
	int bitmap;
	int bitmap_num;

	int act_change_count;

public:
	slime(int x, int y, stage* stage_inf);
	~slime();
	void calc();
	void draw(int camera_x,int camera_y);
};

game_obj* create_enemy(int id, int x, int y, stage* stage_inf);