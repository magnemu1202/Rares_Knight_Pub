#pragma once
#include "Scene.h"
#include "Application.h"
#include "Skill.h"
#include <vector>

class skill_tree : public scene {
private:
	int camera_x;
	int camera_y;

	int** skill_tree_mesh;
	int mesh_column;
	int mesh_line;
	int choose_x;
	int choose_y;
	int cursol_brush;

	int skill_window_bitmap;
	int main_text_font;
	int main_text_brush;
	int sub_text_font;
	int sub_text_brush;
	int cursol_effect_counter;

	scene* origin;
	player* m_player;

	std::vector<skill*> skills;
public:
	skill_tree(scene* origin);
	~skill_tree();
	int init_skill_tree(int x, int y,int id);
	void calc();
	void draw();
};