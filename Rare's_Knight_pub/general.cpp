#include "general.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <codecvt>
#include <locale>


namespace genmod {
	//codecvt‚ª”ñ„§
	std::wstring string_to_wstring(std::string str) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.from_bytes(str);
	}

	//codecvt‚ª”ñ„§
	std::string wstring_to_string(std::wstring wstr) {
		std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
		return converter.to_bytes(wstr);
	}
}