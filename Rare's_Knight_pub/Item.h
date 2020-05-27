#pragma once
#include <string>
#include "Player.h"

class game;
class scene;

class item {
protected:
	int id;
	int num;
	int value;
	std::wstring name;
	std::wstring description;
	int bitmap;

public:
	item(int id);
	virtual ~item() {};
	virtual void use() = 0;
	//�������ߖ�̂��߁A�摜�̓R���X�g���N�^�ŏ����������A�ʂ̃��\�b�h��p�ӂ���B
	void load_bitmap();
	void release_bitmap();
	int get_num() { return this->num; };
	void set_num(int num) { this->num = num; };
	std::wstring get_name() { return this->name; };
	std::wstring get_description() { return this->description; };
};

//id0

class item0 : public item {
public:
	item0(int id);
	~item0();
	void use();
};

item* create_item(int id);