#pragma once
#include <string>
#include <vector>
#include "Scene.h"
#include "Enemy.h"

class game;
class player;
class game_obj;
class event_obj;
class stage_changer;

class stage : public scene {
protected:

    std::vector<game_obj*> objects;
    std::vector<enemy*> enemies;
    std::vector<event_obj*> events;
    std::vector<stage_changer*> changer;
    char** map_data;
    
    std::wstring stage_name;
    int map_column;
    int map_line;

    int camera_x;
    int camera_y;

public:
    stage(int stage_id);
    ~stage();
    virtual void calc() {};
    virtual void draw() {};
    void stack_obj(game_obj* obj);
    void erase_obj(game_obj* obj);
    void enter_stage(int x, int y);
    int get_line_num() { return this->map_line; };
    int get_column_num() { return this->map_column; };
    char** get_mapdata() { return this->map_data; };
    std::vector<enemy*> get_enemy() { return this->enemies; };
    void set_camera_pos(int center_camera_x, int center_camera_y) {
        if (center_camera_x < 400)
            center_camera_x = 400;
        else if (center_camera_x > (this->map_column * 100) - 400)
            center_camera_x = (this->map_column * 100) - 400;
        if (center_camera_y < 300)
            center_camera_y = 300;
        else if (center_camera_y > (this->map_line * 100) - 400)
            center_camera_y = (this->map_line * 100) - 400;
        this->camera_x = center_camera_x - 400;
        this->camera_y = center_camera_y - 300;
    }
};