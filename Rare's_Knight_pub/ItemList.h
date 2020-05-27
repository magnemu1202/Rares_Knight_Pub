#pragma once
#include "Scene.h"
#include <vector>

class game;
class player;
class item;

class item_list : public scene {
private:
	int window_bitmap;
	int cursol_brush;
	int back_ground_brush;
	int hp_bar_brush;
	int player_bitmap;
	int main_text_font;
	int main_text_brush;

	int count;

	int cursol;
	int top;
	std::vector<item*> exist_items;

	scene* origin;

	void update_exist_items();

public:
	item_list(scene* origin);
	~item_list();
	void calc();
	void draw();
};