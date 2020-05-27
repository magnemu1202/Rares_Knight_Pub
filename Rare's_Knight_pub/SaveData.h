#pragma once
#include "Scene.h"

class game;

class save_datas : public scene {
private:
	int window_bitmap;
	int cursor;
	scene* origin;
	bool file_is_normal[5];
	
	int cursol_effect_counter;
	int cursol_brush;
public:
	save_datas(scene* origin);
	~save_datas();
	void calc();
	void draw();
};

class load_datas : public scene {
private:
	int window_bitmap;
	int cursor;
	scene* origin;
	bool file_is_normal[5];

	int cursol_effect_counter;
	int cursol_brush;
public:
	load_datas(scene* origin);
	~load_datas();
	void calc();
	void draw();
};