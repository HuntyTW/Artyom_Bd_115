#include "keyboard_data.hpp"

#include <unordered_map>

using biv::KeyData;
using biv::KeyBoardData;

const std::vector<KeyData> KeyBoardData::KEYS = {
	{192, "Ё"}, {49, "1"}, {50, "2"}, {51, "3"}, {52, "4"}, {53, "5"}, {54, "6"},
		{55, "7"}, {56, "8"}, {57, "9"}, {48, "0"}, {189, "-"}, {187, "="},
	{81, "Й"}, {87, "Ц"}, {69, "У"}, {82, "К"}, {84, "Е"}, {89, "Н"}, {85, "Г"},
		{73, "Ш"}, {79, "Щ"}, {80, "З"}, {219, "Х"}, {221, "Ъ"}, {220, "\\"},
	{65, "Ф"}, {83, "Ы"}, {68, "В"}, {70, "А"}, {71, "П"}, {72, "Р"}, {74, "О"},
		{75, "Л"}, {76, "Д"}, {186, "Ж"}, {222, "Э"},
	{90, "Я"}, {88, "Ч"}, {67, "С"}, {86, "М"}, {66, "И"}, {78, "Т"}, {77, "Ь"},
		{188, "Б"}, {190, "Ю"}, {191, "."}
};

int KeyBoardData::get_code_by_evdev(const int evdev_code) {
	// evdev-код физической клавиши -> код в таблице KEYS
	static const std::unordered_map<int, int> CODES = {
		{41, 192}, {2, 49}, {3, 50}, {4, 51}, {5, 52}, {6, 53}, {7, 54},
			{8, 55}, {9, 56}, {10, 57}, {11, 48}, {12, 189}, {13, 187},
		{16, 81}, {17, 87}, {18, 69}, {19, 82}, {20, 84}, {21, 89}, {22, 85},
			{23, 73}, {24, 79}, {25, 80}, {26, 219}, {27, 221}, {43, 220},
		{30, 65}, {31, 83}, {32, 68}, {33, 70}, {34, 71}, {35, 72}, {36, 74},
			{37, 75}, {38, 76}, {39, 186}, {40, 222},
		{44, 90}, {45, 88}, {46, 67}, {47, 86}, {48, 66}, {49, 78}, {50, 77},
			{51, 188}, {52, 190}, {53, 191}
	};

	const auto it = CODES.find(evdev_code);
	return it == CODES.end() ? -1 : it->second;
}

std::vector<KeyData> KeyBoardData::get_line1() const {
	return {KEYS.begin(), KEYS.begin() + 13};
}

std::vector<KeyData> KeyBoardData::get_line2() const {
	return {KEYS.begin() + 13, KEYS.begin() + 26};
}

std::vector<KeyData> KeyBoardData::get_line3() const {
	return {KEYS.begin() + 26, KEYS.begin() + 37};
}

std::vector<KeyData> KeyBoardData::get_line4() const {
	return {KEYS.begin() + 37, KEYS.begin() + 47};
}
