#pragma once

#include <vector>

#include "key_data.hpp"

namespace biv {
	class KeyBoardData {
		private:
			static const std::vector<KeyData> KEYS;

		public:
			// Коды (Windows Virtual-Key) служебных клавиш
			static constexpr int BACKSPACE_CODE = 8;
			static constexpr int ENTER_CODE = 13;
			static constexpr int SPACE_CODE = 32;

			// Код (Windows Virtual-Key) экранной клавиши по коду физической клавиши
			// Linux (evdev, он же scancode Windows). Если такой клавиши нет - -1.
			static int get_code_by_evdev(const int evdev_code);

			std::vector<KeyData> get_line1() const;
			std::vector<KeyData> get_line2() const;
			std::vector<KeyData> get_line3() const;
			std::vector<KeyData> get_line4() const;
	};
}
