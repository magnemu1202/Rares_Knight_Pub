#pragma once
#include "Scene.h"

class game;

class blank_scene : public scene {
private:
	int counter;
	int blank_count;

	scene* origin;
	scene* next;

	int back_ground_brush;

public:
	blank_scene(scene* origin, scene* next, int blank_count);
	~blank_scene();
	void calc();
	void draw();
};