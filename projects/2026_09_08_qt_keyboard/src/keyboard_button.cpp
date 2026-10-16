#include "keyboard_button.hpp"

#include <QSizePolicy>

using biv::KeyBoardButton;

KeyBoardButton::KeyBoardButton(const QString& text, QWidget* parent)
    : QPushButton(parent) {
	setFont(QFont("Roboto", 20));
	setText(text);
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

	// Кнопки не должны забирать фокус: иначе после клика мышкой пробел и Enter
	// с физической клавиатуры "нажимали" бы последнюю кликнутую кнопку.
	setFocusPolicy(Qt::NoFocus);
}
