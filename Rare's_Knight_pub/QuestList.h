#pragma once
#include "Scene.h"
#include <vector>

class quest;
class game;
class scene;

class quest_list : public scene {
private:
	std::vector<quest*> quests;
	
	int window_bitmap;
	int text_brush;
	int text_font;
	int cursor_brush;
	int cursor;
	int top;

	scene* origin;
public:
	quest_list(scene* origin);
	~quest_list();
	void calc();
	void draw();
};