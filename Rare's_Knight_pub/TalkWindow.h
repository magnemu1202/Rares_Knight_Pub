#pragma once
#include "Scene.h"
#include "Application.h"
#include <string>
#include <vector>

class talk_window : public scene {
private:
	std::vector<std::wstring> talk;
	scene* origin;
	int window_bitmap;
	int talking_itr;
	int text_brush;
	int text_font;

public:
	talk_window(scene* origin,int talk_id);
	~talk_window();
	void calc();
	void draw();
};