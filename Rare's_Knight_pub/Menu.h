#pragma once
#include "Scene.h"

class game;

class menu : public scene {
private:
	int back_ground_brush;
	int main_text_brush;
	int main_text_font;
	int window_bitmap;

	int cursol;
	int cursol_effect_counter;
	int cursol_brush;
	int menu_brush;
	int menu_font;
	scene* origin;
public:
	menu(scene* origin);
	~menu();
	void calc();
	void draw();
};