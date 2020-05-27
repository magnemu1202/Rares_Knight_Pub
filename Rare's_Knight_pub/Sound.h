#pragma once
#include <string>

class game;

class sound {
private:
	int sound_id;
	int sound_data_id;
	bool is_play;
	int volume;
	bool resume;
	unsigned int resume_point;
	unsigned int return_point;

public:
	sound(int sound_data_id);
	~sound();
	void play();
	void stop();
	bool same_sound(int sound_data_id);
	int get_sound_data_id();
};