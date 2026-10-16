#include "welcome_window.hpp"

#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>

using biv::WelcomeWindow;

WelcomeWindow::WelcomeWindow(QWidget* parent) : QWidget(parent) {
	resize(700, 720);
	setWindowTitle("Клавик");

	QLabel* image = new QLabel(this);
	image->setObjectName("welcome_image");
	image->setFixedSize(300, 300);
	image->setPixmap(QPixmap("img/grustnii-smail.png"));
	image->setScaledContents(true);

	QLabel* title = new QLabel("Привет! Я Клавик", this);
	title->setFont(QFont("Roboto", 32));
	title->setAlignment(Qt::AlignCenter);

	QLabel* subtitle = new QLabel(
		"Мне грустно, потому что я ещё не умею печатать.\nПоможешь мне?",
		this
	);
	subtitle->setFont(QFont("Roboto", 18));
	subtitle->setAlignment(Qt::AlignCenter);

	QPushButton* to_keyboard_btn = new QPushButton("Перейти к клавиатуре", this);
	to_keyboard_btn->setObjectName("to_keyboard_button");
	to_keyboard_btn->setFont(QFont("Roboto", 24));
	to_keyboard_btn->setMinimumHeight(80);
	to_keyboard_btn->setFocusPolicy(Qt::NoFocus);

	// Сигнал кнопки clicked пробрасываем в собственный сигнал окна. Что именно
	// делать при переходе, окно не знает: это решает тот, кто подключит слоты.
	connect(to_keyboard_btn, &QPushButton::clicked, this, &WelcomeWindow::keyboard_requested);

	QVBoxLayout* main_layout = new QVBoxLayout(this);
	main_layout->addStretch();
	main_layout->addWidget(image, 0, Qt::AlignHCenter);
	main_layout->addWidget(title);
	main_layout->addWidget(subtitle);
	main_layout->addSpacing(20);
	main_layout->addWidget(to_keyboard_btn);
	main_layout->addStretch();
}

void WelcomeWindow::show_window() {
	show();
	raise();
	activateWindow();
}
