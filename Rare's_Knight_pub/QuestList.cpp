#include "QuestList.h"
#include "Quest.h"
#include "WindowsModule.h"
#include "DirectXModule.h"
#include "sqlite3.h"
#include "Application.h"
#include "Sound.h"
#include "TalkWindow.h"
#include "Player.h"
#include "Item.h"
#include "Enemy.h"
#include <sstream>
#include "ImageManager.h"

quest_list::quest_list(scene* origin) {
	this->origin = origin;
	this->origin->lock();
	this->window_bitmap = image_manager::get_image_id(L"image/window.png");
	this->cursor_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 0.7f);
	this->text_brush = dxmod::d2d_create_brush(1.f, 1.f, 1.f, 1.f);
	this->text_font = dxmod::d2d_create_font(L"ƒƒCƒŠƒI", 14.0f, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL);
	this->top = 0;

	sqlite3* db;
	sqlite3_open("game_datas.db", &db);
	sqlite3_stmt* quest_stmt = nullptr;
	sqlite3_prepare(db, "SELECT * FROM Quest", -1, &quest_stmt, nullptr);
	while (sqlite3_step(quest_stmt) == SQLITE_ROW) {
		quests.push_back(new quest(sqlite3_column_int(quest_stmt, 0)));
	}

}

quest_list::~quest_list() {
	dxmod::d2d_release_brush(this->text_brush);
	dxmod::d2d_release_font(this->text_font);
	for (int i = 0; i < this->quests.size(); i++) {
		delete this->quests[i];
	}
	this->quests.clear();
}

void quest_list::calc() {
	int talk_event_id = game::get_incetance().get_talk_event();
	if (talk_event_id != -1) {
		game::get_incetance().stack_scene(new talk_window(this, talk_event_id));
	}
	if (winmod::get_key_down_state(VK_UP) && this->cursor > 0) {
		game::get_incetance().set_sound(1);
		this->cursor--;
	}
	else if (winmod::get_key_down_state(VK_DOWN) && this->cursor < quests.size()-1) {
		game::get_incetance().set_sound(1);
		this->cursor++;
	}

	if (winmod::get_key_down_state('Z')) {
		game::get_incetance().set_sound(1);
	}
	else if (winmod::get_key_down_state('X')) {
		game::get_incetance().set_sound(1);
		game::get_incetance().erase_scene(this);
		this->origin->unlock();
	}
}

void quest_list::draw() {
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 200, 0, 0, 150, 50);
	for (int i = 0; i < 5; i++) {
		dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 250 + 50 * i, 0, 50, 150, 50);
	}
	dxmod::d2d_draw_part_of_bitmapUL(this->window_bitmap, 5, 500, 0, 100, 150, 50);
	
	for (int i = 0; i < this->top + 6 && i < this->quests.size(); i++) {
		dxmod::d2d_draw_custom_text_string(this->text_font, this->text_brush, 10, 205 + 30 * i, this->quests[i + this->top]->get_name());
	}

	dxmod::d2d_draw_custom_text_string(this->text_font, this->text_brush, 400, 100, this->quests[this->cursor]->get_description());
	std::wstringstream targets;
	targets << L"ƒ^[ƒQƒbƒg" << std::endl;
	for (int i = 0; i < this->quests[this->cursor]->get_targets().size(); i++) {
		targets << game::get_incetance().get_enemies()[this->quests[this->cursor]->get_targets()[i]->enemy_id]->get_name() << L" " << this->quests[this->cursor]->get_targets()[i]->num << L"•C" << std::endl;
	}
	dxmod::d2d_draw_custom_text_string(this->text_font, this->text_brush, 400, 300, targets.str().c_str());
	std::wstringstream rewards;
	rewards << L"•ñV " << this->quests[this->cursor]->get_reawrd_money() << L"z" << std::endl;
	for (int i = 0; i < this->quests[this->cursor]->get_reward_items().size(); i++) {
		rewards << L" " << player::get_incetance()->get_items()[this->quests[this->cursor]->get_reward_items()[i]->item_id]->get_name() << std::endl;
	}
	dxmod::d2d_draw_custom_text_string(this->text_font, this->text_brush, 600, 300, rewards.str().c_str());
}