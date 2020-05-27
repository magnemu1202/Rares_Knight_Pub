#pragma once
#include "GameObj.h"
#include "Application.h"

class scene;

class event_obj : public game_obj {
protected:
	int id;
	int bitmap;

public:
	virtual ~event_obj() {};
	virtual void calc() = 0;
	void talk();
	virtual void draw(int camera_x,int camera_y) = 0;
};

event_obj* create_event(int event_id);

//拡張性を考えてインスタンスによって別の動きをさせたい

//eventid0
class event0 : public event_obj {
public:
	event0();
	~event0();
	void calc();
	void draw(int camera_x,int camera_y);
};

//eventid1
class paper_map : public event_obj {
public:
	paper_map();
	~paper_map();
	void calc();
	void draw(int camera_x, int camera_y);
};