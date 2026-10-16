#pragma once

#include <cstddef>
#include <unordered_map>

#include <QGridLayout>
#include <QString>
#include <QWidget>

#include "keyboard_button.hpp"
#include "keyboard_data.hpp"
#include "key_data.hpp"

namespace biv {
	class KeyBoard : public QWidget {
		Q_OBJECT

		private:
			const int button_width;
			std::unordered_map<int, KeyBoardButton*> buttons;

			KeyBoardData* keyboard_data;

		public:
			KeyBoard(const int width, QWidget* parent = nullptr);

			bool is_key_allowed(const int code) const noexcept;

			// Нажатие клавиши с физической клавиатуры: подсвечивает экранную кнопку
			// и срабатывает ровно так же, как клик по ней мышкой.
			void press_key(const int code);

		signals:
			void symbol_entered(const QString& text);
			void space_entered();
			void enter_entered();
			void backspace_entered();

		private:
			void create_buttons(
				const std::vector<KeyData>& data,
				QGridLayout* layout,
				const int line,
				const int start_position
			);
	};
}
