#include "BlankScene.h"
#include "Application.h"
#include "DirectXModule.h"

blank_scene::blank_scene(scene* origin, scene* next, int blank_count) {
	this->origin = origin;
	this->origin->lock();
	this->next = next;
	this->blank_count = blank_count + 60;
	this->counter = 0;
	this->back_ground_brush = dxmod::d2d_create_brush(0, 0, 0, 0);
}

blank_scene::~blank_scene() {
	dxmod::d2d_release_brush(this->back_ground_brush);
}

void blank_scene::calc() {
	this->counter++;
	if (this->counter < 30) {
		dxmod::d2d_release_brush(this->back_ground_brush);
		this->back_ground_brush = dxmod::d2d_create_brush(0, 0, 0, 1.f - (30.f - this->counter) * (1.f / 30.f));
	}
	else if (this->counter == 30) {
		game::get_incetance().erase_scene(this->origin);
	}
	else if (this->counter == this->blank_count - 30) {
		game::get_incetance().enqueue_scene(this->next);
		this->next->lock();
	}
	else if (this->counter > this->blank_count - 30 && this->counter < this->blank_count) {
		dxmod::d2d_release_brush(this->back_ground_brush);
		this->back_ground_brush = dxmod::d2d_create_brush(0, 0, 0, 1.f - (30.f - (this->blank_count - this->counter)) * (1 / 30.f));
	}
	else if(this->counter == this->blank_count) {
		this->next->unlock();
		game::get_incetance().erase_scene(this);
	}
}

void blank_scene::draw() {
	dxmod::d2d_draw_rect(this->back_ground_brush, 0, 0, 800, 600, 0, true);
}