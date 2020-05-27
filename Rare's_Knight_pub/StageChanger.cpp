#include "StageChanger.h"
#include "DirectXModule.h"
#include "WindowsModule.h"
#include "ImageManager.h"
#include "Application.h"
#include "Player.h"
#include "BlankScene.h"
#include "Map.h"
#include "BlankScene.h"
#include "Field.h"
#include "Pub.h"
#include "Home.h"

stage_changer::stage_changer(int x, int y, scene* origin, int next_scene_id,int dist_x,int dist_y) {
	this->x = x;
	this->y = y;
	this->origin = origin;
	this->next_scene_id = next_scene_id;
	this->dist_x = dist_x;
	this->dist_y = dist_y;
}

stage_changer::~stage_changer() {
	
}

field_stage_changer::field_stage_changer(int x, int y, scene* origin, int next_scene_id,int dist_x,int dist_y) : stage_changer(x, y, origin, next_scene_id,dist_x,dist_y) {
	this->bitmap = image_manager::get_image_id(L"image/field_stage_changer.png");
}

field_stage_changer::~field_stage_changer() {
	image_manager::release_image(this->bitmap);
}

void field_stage_changer::calc() {
	this->can_enter = std::abs(player::get_incetance()->get_x() - this->x) < 50 && std::abs(player::get_incetance()->get_y() - this->y) < 100;
	if (winmod::get_key_down_state('Z') && this->can_enter) {
		if (this->next_scene_id == -1) {
			map* next = new map();
			blank_scene* blank = new blank_scene(this->origin, next, 60);
			game::get_incetance().stack_scene(blank);
		}
		else {
			stage* next = new field(this->next_scene_id);
			blank_scene* blank = new blank_scene(this->origin, next, 60);
			player::get_incetance()->exit_stage();
			player::get_incetance()->enter_stage(next);
			player::get_incetance()->set_x(this->dist_x);
			player::get_incetance()->set_y(this->dist_y);
			next->set_camera_pos(this->dist_x, this->dist_y);
			game::get_incetance().stack_scene(blank);
		}
	}
}

void field_stage_changer::draw(int camera_x,int camera_y) {
	dxmod::d2d_draw_bitmap(this->bitmap, this->x - camera_x, this->y - camera_y);
	if (this->can_enter) {
		dxmod::d2d_draw_string(600, 560, L"キーでマップ移動");
	}
}

pub_enter_door::pub_enter_door(int x, int y, scene* origin, int next_scene_id, int dist_x, int dist_y) : stage_changer(x, y, origin, next_scene_id, dist_x, dist_y) {
	this->bitmap = image_manager::get_image_id(L"image/door.png");
}

pub_enter_door::~pub_enter_door() {
	image_manager::release_image(this->bitmap);
}

void pub_enter_door::calc() {
	this->can_enter = std::abs(player::get_incetance()->get_x() - this->x) < 50 && std::abs(player::get_incetance()->get_y() - this->y) < 100;
	if (winmod::get_key_down_state('Z') && this->can_enter) {
		stage* dist = new home();
		blank_scene* blank = new blank_scene(this->origin, dist, 60);
		player::get_incetance()->exit_stage();
		player::get_incetance()->enter_stage(dist);
		player::get_incetance()->set_x(this->dist_x);
		player::get_incetance()->set_y(this->dist_y);
		dist->set_camera_pos(this->dist_x, this->dist_y);
		game::get_incetance().stack_scene(blank);
	}
}

void pub_enter_door::draw(int camera_x, int camera_y) {
	dxmod::d2d_draw_bitmap(this->bitmap, this->x - camera_x, this->y - camera_y);
	if (this->can_enter) {
		dxmod::d2d_draw_string(600, 560, L"キーでマップ移動");
	}
}


pub_exit_door::pub_exit_door(int x, int y, scene* origin, int next_scene_id, int dist_x, int dist_y) : stage_changer(x, y, origin, next_scene_id, dist_x, dist_y) {
	this->bitmap = image_manager::get_image_id(L"image/door.png");
}

pub_exit_door::~pub_exit_door() {
	image_manager::release_image(this->bitmap);
}

void pub_exit_door::calc() {
	this->can_enter = std::abs(player::get_incetance()->get_x() - this->x) < 50 && std::abs(player::get_incetance()->get_y() - this->y) < 100;
	if (winmod::get_key_down_state('Z') && this->can_enter) {
		pub* dist = new pub();
		blank_scene* blank = new blank_scene(this->origin, dist, 60);
		player::get_incetance()->exit_stage();
		player::get_incetance()->enter_stage(dist);
		player::get_incetance()->set_x(this->dist_x);
		player::get_incetance()->set_y(this->dist_y);
		dist->set_camera_pos(this->dist_x, this->dist_y);
		game::get_incetance().stack_scene(blank);
	}
}

void pub_exit_door::draw(int camera_x, int camera_y) {
	dxmod::d2d_draw_bitmap(this->bitmap, this->x - camera_x, this->y - camera_y);
	if (this->can_enter) {
		dxmod::d2d_draw_string(600, 560, L"キーでマップ移動");
	}
}

pub_exit::pub_exit(int x, int y, scene* origin, int next_scene_id, int dist_x, int dist_y) : stage_changer(x, y, origin, next_scene_id, dist_x, dist_y) {
	
}

pub_exit::~pub_exit() {
	
}

void pub_exit::calc() {
	this->can_enter = std::abs(player::get_incetance()->get_x() - this->x) < 50 && std::abs(player::get_incetance()->get_y() - this->y) < 100;
	if (winmod::get_key_down_state('Z') && this->can_enter) {
		map* dist = new map();
		blank_scene* blank = new blank_scene(this->origin, dist, 60);
		game::get_incetance().stack_scene(blank);
	}
}

void pub_exit::draw(int camera_x, int camera_y) {
	if (this->can_enter) {
		dxmod::d2d_draw_string(600, 560, L"キーでマップ移動");
	}
}