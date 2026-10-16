#include "keyboard_window.hpp"

#include <QHBoxLayout>
#include <QTextCursor>
#include <QVBoxLayout>

using biv::KeyBoardWindow;

KeyBoardWindow::KeyBoardWindow(QWidget* parent) : QWidget(parent) {
	const int keyboard_width = 1160;
	resize(keyboard_width, 770);

	// Окно само принимает нажатия с физической клавиатуры (все дочерние
	// виджеты фокус не берут), поэтому фокус ставим на него.
	setFocusPolicy(Qt::StrongFocus);

	sad_pixmap = QPixmap("img/grustnii-smail.png");
	happy_pixmap = QPixmap("img/veselii-smail.png");

	image = new QLabel(this);
	image->setObjectName("image");
	image->setFixedSize(200, 200);
	image->setScaledContents(true);

	mood_label = new QLabel(this);
	mood_label->setObjectName("mood_label");
	mood_label->setFont(QFont("Roboto", 28));
	mood_label->setWordWrap(true);
	mood_label->setAlignment(Qt::AlignCenter);

	QPushButton* back_btn = new QPushButton("Назад", this);
	back_btn->setObjectName("back_button");
	back_btn->setFont(QFont("Roboto", 20));
	back_btn->setMinimumSize(150, 60);
	back_btn->setFocusPolicy(Qt::NoFocus);
	connect(back_btn, &QPushButton::clicked, this, &KeyBoardWindow::welcome_requested);

	QHBoxLayout* smail_layout = new QHBoxLayout();
	smail_layout->addWidget(image);
	smail_layout->addWidget(mood_label, 1);
	smail_layout->addWidget(back_btn, 0, Qt::AlignTop);

    display = new QPlainTextEdit();
	display->setObjectName("display");
	display->setMinimumHeight(140);
	display->setFont(QFont("Roboto", 32));
    display->setReadOnly(true);
	display->setFocusPolicy(Qt::NoFocus);
	display->setPlaceholderText("Помоги мне заработать лучше...");

	keyboard = new KeyBoard(keyboard_width);

	// Все нажатия (и мышкой, и с физической клавиатуры) приходят от клавиатуры
	// в виде сигналов, а окно обрабатывает их в слотах.
	connect(keyboard, &KeyBoard::symbol_entered, this, &KeyBoardWindow::add_symbol);
	connect(keyboard, &KeyBoard::space_entered, this, &KeyBoardWindow::add_space);
	connect(keyboard, &KeyBoard::enter_entered, this, &KeyBoardWindow::add_enter);
	connect(keyboard, &KeyBoard::backspace_entered, this, &KeyBoardWindow::remove_symbol);

    QVBoxLayout* main_layout = new QVBoxLayout(this);
	main_layout->addLayout(smail_layout);
    main_layout->addWidget(display);
    main_layout->addWidget(keyboard);

	update_mood();
}

void KeyBoardWindow::show_window() {
	show();
	raise();
	activateWindow();
	setFocus();
}

void KeyBoardWindow::add_symbol(const QString& text) {
	display->moveCursor(QTextCursor::End);
	display->insertPlainText(text);
	update_mood();
}

void KeyBoardWindow::add_space() {
	add_symbol(" ");
}

void KeyBoardWindow::add_enter() {
	add_symbol("\n");
}

void KeyBoardWindow::remove_symbol() {
	display->moveCursor(QTextCursor::End);

	QTextCursor cursor = display->textCursor();
	cursor.deletePreviousChar();
	display->setTextCursor(cursor);

	update_mood();
}

// ----------------------------------------------------------------------------
// 									PROTECTED
// ----------------------------------------------------------------------------
void KeyBoardWindow::keyPressEvent(QKeyEvent* event) {
	const int code = get_key_code(event);
	if (keyboard->is_key_allowed(code)) {
		keyboard->press_key(code);
	} else {
		QWidget::keyPressEvent(event);
	}
}

// ----------------------------------------------------------------------------
// 									PRIVATE
// ----------------------------------------------------------------------------
int KeyBoardWindow::get_key_code(const QKeyEvent* event) const {
	// Служебные клавиши определяем по переносимым кодам Qt,
	// остальные (буквы, цифры, знаки) - по коду физической клавиши.
	switch (event->key()) {
		case Qt::Key_Backspace:
			return KeyBoardData::BACKSPACE_CODE;
		case Qt::Key_Return:
		case Qt::Key_Enter:
			return KeyBoardData::ENTER_CODE;
		case Qt::Key_Space:
			return KeyBoardData::SPACE_CODE;
		default:
			return event->nativeVirtualKey();
	}
}

void KeyBoardWindow::update_mood() {
	// Пока ничего не написано - Клавику грустно, как только пошёл текст - он рад.
	const bool is_happy = !display->document()->isEmpty();

	image->setPixmap(is_happy ? happy_pixmap : sad_pixmap);
	mood_label->setText(
		is_happy ? "Ура! Мне уже лучше, продолжай!" : "Мне грустно... Напиши что-нибудь!"
	);
	setWindowTitle(is_happy ? "Весёлая Клавиатура" : "Грустная Клавиатура");
}
