#pragma once
#include <vector>
#include <string>

class player;

class node {
public:
	int origin;
	int next;
};

class skill {
private:
	int id;

	std::wstring main_text;
	std::wstring sub_text;

	int p_hp;
	int p_atk;
	int p_def;
	int p_spd;

	bool active;
	player* m_player;

	std::vector<node*> nodes;

public:
	skill(int id, player* m_player);
	~skill();
	std::wstring get_main_text() { return this->main_text; };
	std::wstring get_sub_text() { return this->sub_text; };
	std::vector<node*> get_nodes() { return this->nodes; };
	void init();
	void activate();
	void init_skill_param();
	bool is_active();
	
};