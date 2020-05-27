#pragma once

class game_obj {
protected:
	int x;
	int y;
public:
	virtual ~game_obj() {};
	int get_x() { return x; };
	int get_y() { return y; };
	void set_x(int x) { this->x = x; };
	void set_y(int y) { this->y = y; };
	virtual void calc() = 0;
	virtual void draw(int camera_x, int camera_y) = 0;
};