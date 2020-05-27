#include "pub.h"
#include "DirectXModule.h"
#include "WindowsModule.h"
#include "general.h"
#include "sqlite3.h"
#include "Player.h"
#include "Application.h"
#include "TalkWindow.h"
#include "Menu.h"
#include <sstream>
#include "ImageManager.h"
#include "StageChanger.h"

pub::pub() : stage(1) {

	this->front_image = image_manager::get_image_id(L"image/pub_front.png");
	this->back_image = image_manager::get_image_id(L"image/pub_back.png");
	this->back_sky_bitmap = image_manager::get_image_id(L"image/back_sky0.png");

	pub_enter_door* home_door = new pub_enter_door(900, 500, this, 1, 900, 500);
	this->objects.insert(this->objects.begin(), home_door);
	this->changer.push_back(home_door);

	pub_exit* exit = new pub_exit(100, 500, this, 1, 0, 0);
	this->objects.insert(this->objects.begin(), exit);
	this->changer.push_back(exit);

}

pub::~pub() {
	for (int i = 0; i < objects.size(); i++) {
		delete objects[i];
	}
	objects.clear();
	image_manager::release_image(this->front_image);
	image_manager::release_image(this->back_sky_bitmap);
	image_manager::release_image(this->back_image);
	for (int i = 0; i < this->map_line; i++)
		delete[] map_data[i];
	delete[] map_data;
}

void pub::calc() {
	int talk_event_id = game::get_incetance().get_talk_event();
	if (talk_event_id != -1) {
		game::get_incetance().stack_scene(new talk_window(this, talk_event_id));
	}

	player::get_incetance()->calc();

	if (winmod::get_key_down_state('X')) {
		game::get_incetance().stack_scene(new menu(this));
	}

	int size = objects.size();
	for (int i = 0; i < size; i++) {
		objects[i]->calc();
	}
}

void pub::draw() {
	//îwåiï`âÊ
	dxmod::d2d_draw_bitmapUL(this->back_sky_bitmap, 0, 0);
	dxmod::d2d_draw_bitmapUL(this->back_image, -1 * camera_x, -1 * camera_y);

	//ÉIÉuÉWÉFÉNÉgï`âÊ

	int size = objects.size();
	for (int i = 0; i < size; i++) {
		objects[i]->draw(camera_x, camera_y);
	}

	player::get_incetance()->draw(this->camera_x, this->camera_y);

	dxmod::d2d_draw_bitmapUL(this->front_image, -1 * camera_x, -1 * camera_y);

	dxmod::d2d_draw_string(10, 10, L"HP:");
	dxmod::d2d_draw_rect(0, 50, 20, player::get_incetance()->get_maxhp() + player::get_incetance()->get_skill_maxhp() + 50, 25, 1, false);
	dxmod::d2d_draw_rect_with_dynamicscb(1, 1, 1, 0.7, player::get_incetance()->get_hp() + 50, 20, 50, 25);
}