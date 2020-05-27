#include "SkillTree.h"
#include "Skill.h"
#include "Player.h"
#include "DirectXModule.h"
#include "WindowsModule.h"
#include <cmath>
#include "ImageManager.h"

skill_tree::skill_tree(scene* origin_scene) {
	this->skills = player::get_incetance()->get_skills();
	
	this->mesh_column = 10;
	this->mesh_line = 5;
	this->choose_x = this->choose_y = 0;
	this->camera_x = this->camera_y = 0;

	this->skill_window_bitmap = image_manager::get_image_id(L"image/skill_window.png");
	this->main_text_font = dxmod::d2d_create_font(L"„M‚r@ƒSƒVƒbƒN", 32.0f, DWRITE_FONT_WEIGHT_LIGHT, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);
	this->main_text_brush = dxmod::d2d_create_brush(1.0, 1.0, 1.0,1.0);

	this->skill_tree_mesh = new int* [this->mesh_line];
	for (int i = 0; i < this->mesh_line; i++) {
		this->skill_tree_mesh[i] = new int[this->mesh_column];
	}

	//this->choose_box_brush[0] = dxmod::d2d_create_brush(1.0, 1.0, 1.0, 0.7);

	//-1 init
	for (int i = 0; i < this->mesh_column; i++) {
		for (int j = 0; j < this->mesh_line; j++) {
			this->skill_tree_mesh[j][i] = -1;
		}
	}

	this->init_skill_tree(0, 0, 0);
	
	this->cursol_effect_counter = 0;

	this->origin = origin_scene;
	this->origin->lock();
}

int skill_tree::init_skill_tree(int x,int y,int id) {
	this->skill_tree_mesh[y][x] = id;
	int line = 0;
	for (int i = 0; i < this->skills[id]->get_nodes().size(); i++) {
		line = this->init_skill_tree(x + 1, y + i + line, this->skills[id]->get_nodes()[i]->next);
	}
	return line;
}

skill_tree::~skill_tree() {
	for (int i = 0; i < this->mesh_line; i++)
		delete[] this->skill_tree_mesh[i];
	delete[] this->skill_tree_mesh;
}

void skill_tree::calc() {
	this->cursol_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, (0.25*std::sin((++this->cursol_effect_counter) / 30.f)) + 0.5);
	if (winmod::get_key_down_state(VK_LEFT) && this->choose_x != 0) {
		for (int i = this->choose_x -1; i >= 0; i--) {
			if (this->skill_tree_mesh[this->choose_y][i] != -1) {
				this->choose_x = i;
				break;
			}
		}
	}
	else if (winmod::get_key_down_state(VK_RIGHT) && this->choose_x != this->mesh_column-1) {
		for (int i = this->choose_x + 1; i < this->mesh_column; i++) {
			if (this->skill_tree_mesh[this->choose_y][i] != -1) {
				this->choose_x = i;
				break;
			}
		}
	}
	if (winmod::get_key_down_state(VK_UP) && this->choose_y != 0) {
		for (int i = this->choose_y -1; i >= 0; i--) {
			if (this->skill_tree_mesh[i][this->choose_x] != -1) {
				this->choose_y = i;
				break;
			}
		}
	}
	else if (winmod::get_key_down_state(VK_DOWN) && this->choose_y != this->mesh_line -1) {
		for (int i = this->choose_y + 1; i < this->mesh_line; i++) {
			if (this->skill_tree_mesh[i][this->choose_x] != -1) {
				this->choose_y = i;
				break;
			}
		}
	}
	if (winmod::get_key_down_state('Z') && !(this->skills[this->skill_tree_mesh[this->choose_y][this->choose_y]]->is_active())) {
		this->skills[this->skill_tree_mesh[this->choose_y][this->choose_x]]->activate();
	}
	if (winmod::get_key_down_state('X')) {
		game::get_incetance().erase_scene(this);
		this->origin->unlock();
	}

	if (this->camera_x < 0) {
		this->camera_x = 0;
	}
	if (this->choose_y < 0) {
		this->camera_y = 0;
	}
}

void skill_tree::draw() {
	for (int i = 0; i < 5; i++) {
		for (int j = 0; j < 4; j++) {
			int cameraed_i = i + this->camera_x / 200;
			int cameraed_j = j + this->camera_y / 200;
			if (cameraed_i < 0 || cameraed_i >= this->mesh_column || cameraed_j < 0 || cameraed_j >= this->mesh_line)
				break;
			else if (this->skill_tree_mesh[cameraed_j][cameraed_i] != -1) {
				dxmod::d2d_draw_bitmapUL(this->skill_window_bitmap, 50 + cameraed_i * 250 - this->camera_x, 50 + cameraed_j * 150 - this->camera_y);
				dxmod::d2d_draw_custom_text_string(0, this->main_text_brush, 50 + cameraed_i * 250 - this->camera_x, 100 + cameraed_j * 150 - this->camera_y,this->skills[this->skill_tree_mesh[cameraed_j][cameraed_i]]->get_main_text());
				int null_mesh_num = 0;
				for (int k = 0; k < this->skills[this->skill_tree_mesh[cameraed_j][cameraed_i]]->get_nodes().size(); k++) {
					while (skill_tree_mesh[cameraed_j + k+null_mesh_num][cameraed_i + 1] == -1) {
						null_mesh_num++;
					}
					dxmod::d2d_draw_line(0, 250 + cameraed_i * 250 - this->camera_x, 100 + cameraed_j * 150 - this->camera_y, 300 + cameraed_i * 250 - this->camera_x, 100 + (cameraed_j+k+null_mesh_num) * 150 - this->camera_y,2.f);
				}
			}
		}
	}
	int left = 50 + (choose_x + this->camera_x / 200) * 250 - this->camera_x;
	int top = 50 + (choose_y + this->camera_y / 200) * 150 - this->camera_y;
	dxmod::d2d_draw_rect(this->cursol_brush, left, top,left + 200,top+100,1,true);
}