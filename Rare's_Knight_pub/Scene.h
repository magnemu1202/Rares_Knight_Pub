#pragma once

class scene {
private:
	bool scene_locker;
public:
	virtual ~scene() {};
	void lock() {
		scene_locker = true;
	};
	void unlock() {
		scene_locker = false;
	}
	bool is_locked() {
		return scene_locker;
	}
	virtual void calc() = 0;
	virtual void draw() = 0;
};