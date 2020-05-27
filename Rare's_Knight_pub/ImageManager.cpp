#include "ImageManager.h"

std::map<std::wstring, int> image_manager::container;
std::map<int, int> image_manager::ref_nums;

int image_manager::get_image_id(std::wstring file_name) {
	auto itr = container.find(file_name);
	if (itr == container.end()) {
		int bitmap_id = dxmod::d2d_load_png_from_file(file_name.c_str());
		container.insert(std::make_pair(file_name, bitmap_id));
		ref_nums.insert(std::make_pair(bitmap_id, 1));
		return bitmap_id;
	}
	else {
		int bitmap_id = itr->second;
		ref_nums[bitmap_id]++;
		return bitmap_id;
	}
}

void image_manager::release_image(int id) {
	//要素の逆引きを行い、valueから要素を求め、画像の削除とmapの削除を行う。
	for (auto itr = container.begin(); itr != container.end(); itr++) {
		if (itr->second == id) {
			if (ref_nums[itr->second] <= 1) {
				dxmod::d2d_release_bitmap(itr->second);
				ref_nums.erase(ref_nums.find(itr->second));
				container.erase(itr);
			}
			else {
				ref_nums[itr->second]--;
			}
			break;
		}
	}
}