#pragma once
#include <string>
#include <vector>


class game;
class player;
class item;
class enemy;

class target {
public:
	int enemy_id;
	int num;
};

class reward_item {
public:
	int item_id;
	int num;
};

class quest {
private:
	;

	std::wstring name;
	std::wstring description;
	bool can_re_order;
	bool can_order;
	int reward_exp;
	int reward_money;
	std::vector<reward_item*> reward_items;
	std::vector<target*> targets;

public:
	std::wstring get_name() { return this->name; };
	std::wstring get_description() { return this->description; };
	std::vector<reward_item*> get_reward_items() { return this->reward_items; };
	std::vector<target*> get_targets() { return this->targets; };
	int get_reawrd_money() { return this->reward_money; };
	int get_reward_exp() { return this->reward_exp; };
	quest(int id);
	~quest();
	void check_dead_enemy(enemy* dead_enemy);
	bool is_achived();
	void achive();
};