#pragma once
#include "Application.h"
#include "Scene.h"
#include <string>

class map_stage_inf {
public:
	std::wstring stage_name;
	std::wstring stage_exp;
	int x;
	int y;
	~map_stage_inf() {
		stage_name.clear();
		stage_exp.clear();
	}
};

class map : public scene {
private:
	int map_bitmap;
	int map_point_bitmap;
	int window_bitmap;

	int bitmap_num;
	int bitmap_change_count;

	std::vector<map_stage_inf*> stage_inf;

	int chosen_point;

public:
	map();
	~map();
	void calc();
	void draw();
};