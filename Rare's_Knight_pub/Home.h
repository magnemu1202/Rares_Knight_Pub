#pragma once
#include "Stage.h"

class home : public stage {
private:
	int back_sky_bitmap;
	int back_image;

public:
	home();
	~home();
	void calc();
	void draw();
};