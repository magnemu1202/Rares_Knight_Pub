#pragma once
#include "Stage.h"

class pub : public stage {
private:
	int back_sky_bitmap;
	int back_image;
	int front_image;

public:
	pub();
	~pub();
	void calc();
	void draw();
};