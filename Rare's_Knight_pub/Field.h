#pragma once

#include "Stage.h"

class field : public stage {
private:
	int background_bitmap;
	int back_sky_bitmap;
	int map_chip;

public:
	field(int stage_id);
	~field();
	void calc();
	void draw();
};