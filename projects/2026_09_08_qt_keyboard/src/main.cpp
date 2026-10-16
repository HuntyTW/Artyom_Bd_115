#include <QApplication>

#include "keyboard_window.hpp"
#include "welcome_window.hpp"

int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    biv::WelcomeWindow welcome_window;
    biv::KeyBoardWindow keyboard_window;

	// Переходы между окнами: сначала показываем новое окно, потом прячем старое
	// (чтобы в какой-то момент не оказалось ни одного видимого окна).
	QObject::connect(
		&welcome_window, &biv::WelcomeWindow::keyboard_requested,
		&keyboard_window, &biv::KeyBoardWindow::show_window
	);
	QObject::connect(
		&welcome_window, &biv::WelcomeWindow::keyboard_requested,
		&welcome_window, &QWidget::hide
	);

	QObject::connect(
		&keyboard_window, &biv::KeyBoardWindow::welcome_requested,
		&welcome_window, &biv::WelcomeWindow::show_window
	);
	QObject::connect(
		&keyboard_window, &biv::KeyBoardWindow::welcome_requested,
		&keyboard_window, &QWidget::hide
	);

    welcome_window.show();

    return app.exec();
}
