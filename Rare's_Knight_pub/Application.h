#pragma once
#include <vector>

class player;
class scene;
class sound;
class quest;
class enemy;

//ストーリーフラグの番号
#define QUEST1_CLEARD 0;

class game {
private:
	//sceneのstack行列
	std::vector<scene*> scenes;
	std::vector<scene*> next_erase_scene;
	std::vector<scene*> next_stack_scene;
	std::vector<scene*> next_enqueue_scene;
	std::vector<int> talks;
	std::vector<sound*> sounds;
	std::vector<quest*> active_quests;
	std::vector<enemy*> enemies;
	std::vector<bool> story_flags;
	//一回読み取られたら破棄する一時的なフラグ
	std::vector<int> temp_flags;

public:
	game();
	~game();
	std::vector<quest*> get_active_quests();
	std::vector<enemy*> get_enemies() { return this->enemies; };
	void calculation();
	void draw();
	void erase_scene(scene* erased_scene);
	void stack_scene(scene* s_scene);
	void enqueue_scene(scene* e_scene);
	void stack_talk_event(int talk_id);
	void set_sound(int sound_data_id);
	void erase_sound(int sound_data_id);
	void reset_sound();
	bool check_temp_flag(int flag_num);
	int get_talk_event();
	void save_data(int file_id);
	void load_data(int file_id);
	bool check_save_data(int file_id);
	void load_game();

	static game& get_incetance(){
		static game incetance;
		return incetance;
	}
};