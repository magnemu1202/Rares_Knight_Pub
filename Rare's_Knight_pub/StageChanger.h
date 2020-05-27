#pragma once
#include "GameObj.h"
#include <string>

class scene;
class game;

class stage_changer : public game_obj {
protected:
	scene* origin;
	int next_scene_id;
	bool can_enter;
	int dist_x;
	int dist_y;

public:
	stage_changer(int x, int y, scene* origin, int next_scene_id,int dist_x,int dist_y);
	virtual ~stage_changer();
	virtual void calc() = 0;
	virtual void draw(int camera_x, int camera_y) = 0;
};

class field_stage_changer : public stage_changer {
private:
	int bitmap;
public:
	field_stage_changer(int x, int y, scene* origin, int next_scene_id, int dist_x, int dist_y);
	~field_stage_changer();
	void calc();
	void draw(int camera_x, int camera_y);
};

class pub_enter_door : public stage_changer {
private:
	int bitmap;
public:
	pub_enter_door(int x, int y, scene* origin, int next_scene_id, int dist_x, int dist_y);
	~pub_enter_door();
	void calc();
	void draw(int camera_x, int camera_y);
};

class pub_exit_door : public stage_changer {
private:
	int bitmap;
public:
	pub_exit_door(int x, int y, scene* origin, int next_scene_id, int dist_x, int dist_y);
	~pub_exit_door();
	void calc();
	void draw(int camera_x, int camera_y);
};

class pub_exit : public stage_changer {
public:
	pub_exit(int x, int y, scene* origin, int next_scene_id, int dist_x, int dist_y);
	~pub_exit();
	void calc();
	void draw(int camera_x, int camera_y);
};