#pragma once
#include "Scene.h"

class game;

class title : public scene {
private:
	int bitmap;
	int cursol;
	int cursol_effect_counter;

	int window_bitmap;
	int cursol_brush;
	int text_brush;
	int text_font;

public:
	title();
	~title();
	void calc();
	void draw();
};