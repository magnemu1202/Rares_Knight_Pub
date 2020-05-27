#pragma once
#include "DirectXModule.h"
#include <map>
#include <string>

class image_manager {
private:
	static std::map<std::wstring, int> container;
	static std::map<int, int> ref_nums;

public:
	static int get_image_id(std::wstring file_name);

	static void release_image(int id);
};