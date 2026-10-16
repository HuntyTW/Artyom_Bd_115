#include <QApplication>
#include <QElapsedTimer>
#include <QKeyEvent>
#include <QLabel>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QSignalSpy>
#include <QTimer>
#include <QtTest>

#include "keyboard_button.hpp"
#include "keyboard_window.hpp"
#include "welcome_window.hpp"

using biv::KeyBoardButton;
using biv::KeyBoardWindow;
using biv::WelcomeWindow;

// Запускать из папки проекта (картинки загружаются из img/).
class TestKeyBoard : public QObject {
	Q_OBJECT

	private:
		static KeyBoardButton* find_key(QWidget* window, const QString& text) {
			for (KeyBoardButton* btn: window->findChildren<KeyBoardButton*>()) {
				if (btn->text() == text) {
					return btn;
				}
			}
			return nullptr;
		}

		static QString text_of(QWidget* window) {
			return window->findChild<QPlainTextEdit*>("display")->toPlainText();
		}

		// Нажатие с физической клавиатуры: Windows Virtual-Key + код Qt
		static void press_physical(QWidget* window, const int qt_key, const int virtual_key) {
			QKeyEvent event(QEvent::KeyPress, qt_key, Qt::NoModifier, 0, virtual_key, 0);
			QApplication::sendEvent(window, &event);
		}

	private slots:
		void welcome_button_emits_signal() {
			WelcomeWindow welcome;
			welcome.show();
			QSignalSpy spy(&welcome, &WelcomeWindow::keyboard_requested);

			QPushButton* btn = welcome.findChild<QPushButton*>("to_keyboard_button");
			QVERIFY(btn);
			QCOMPARE(btn->text(), QString("Перейти к клавиатуре"));

			QTest::mouseClick(btn, Qt::LeftButton);
			QCOMPARE(spy.count(), 1);
		}

		void mouse_letters_digits_space_enter_backspace() {
			KeyBoardWindow window;
			window.show();

			const QStringList keys = {"Й", "Ц", "1", "0", "Я"};
			for (const QString& key: keys) {
				KeyBoardButton* btn = find_key(&window, key);
				QVERIFY2(btn, qPrintable("нет кнопки " + key));
				QTest::mouseClick(btn, Qt::LeftButton);
			}
			QCOMPARE(text_of(&window), QString("ЙЦ10Я"));

			QTest::mouseClick(find_key(&window, ""), Qt::LeftButton);
			QCOMPARE(text_of(&window), QString("ЙЦ10Я "));

			QTest::mouseClick(find_key(&window, "Enter"), Qt::LeftButton);
			QCOMPARE(text_of(&window), QString("ЙЦ10Я \n"));

			KeyBoardButton* backspace = find_key(&window, "Удалить");
			QTest::mouseClick(backspace, Qt::LeftButton);
			QTest::mouseClick(backspace, Qt::LeftButton);
			QCOMPARE(text_of(&window), QString("ЙЦ10Я"));
		}

		void physical_keys_work_like_mouse() {
			KeyBoardWindow window;
			window.show();

			press_physical(&window, Qt::Key_Q, 81);  // Й
			press_physical(&window, Qt::Key_1, 49);  // 1
			press_physical(&window, Qt::Key_Space, 0);
			press_physical(&window, Qt::Key_Return, 0);
			press_physical(&window, Qt::Key_Z, 90);  // Я
			QCOMPARE(text_of(&window), QString("Й1 \nЯ"));

			press_physical(&window, Qt::Key_Backspace, 0);
			press_physical(&window, Qt::Key_Backspace, 0);
			QCOMPARE(text_of(&window), QString("Й1 "));

			// Клавиша, которой нет на клавиатуре, ничего не вводит
			press_physical(&window, Qt::Key_F5, 116);
			QCOMPARE(text_of(&window), QString("Й1 "));
		}

#ifndef Q_OS_WIN
		// Linux: клавиша определяется по scancode (evdev + 8), а не по символу,
		// поэтому при русской раскладке результат тот же.
		void physical_keys_by_scancode_on_linux() {
			KeyBoardWindow window;
			window.show();

			auto press_scancode = [&](const int qt_key, const int evdev, const QString& text) {
				QKeyEvent event(QEvent::KeyPress, qt_key, Qt::NoModifier, evdev + 8, 0, 0, text);
				QApplication::sendEvent(&window, &event);
			};

			press_scancode(0x419, 16, "й");           // русская раскладка, клавиша Q
			press_scancode(Qt::Key_Q, 16, "q");       // английская раскладка, клавиша Q
			press_scancode(Qt::Key_Period, 52, ".");   // клавиша "Ю"
			press_scancode(Qt::Key_Period, 53, ".");   // клавиша "." (русская раскладка)
			press_scancode(Qt::Key_5, 6, "5");
			QCOMPARE(text_of(&window), QString("ЙЙЮ.5"));
		}
#endif

		void physical_key_presses_each_time_even_when_fast() {
			KeyBoardWindow window;
			window.show();

			for (int i = 0; i < 5; i++) {
				press_physical(&window, Qt::Key_A, 65);  // Ф
			}
			QCOMPARE(text_of(&window), QString("ФФФФФ"));
		}

		void mouse_click_does_not_steal_keyboard_input() {
			KeyBoardWindow window;
			window.show();

			QTest::mouseClick(find_key(&window, "Й"), Qt::LeftButton);
			press_physical(&window, Qt::Key_Space, 0);
			// Пробел - это пробел, а не повторное нажатие на последнюю кликнутую кнопку
			QCOMPARE(text_of(&window), QString("Й "));
		}

		void klavik_gets_happy_when_text_appears() {
			KeyBoardWindow window;
			window.show();

			QLabel* image = window.findChild<QLabel*>("image");
			QLabel* mood = window.findChild<QLabel*>("mood_label");
			QVERIFY(image && mood);
			QVERIFY(!image->pixmap().isNull());

			const QImage sad_image = image->pixmap().toImage();
			const QString sad_text = mood->text();
			const QString sad_title = window.windowTitle();

			QTest::mouseClick(find_key(&window, "Й"), Qt::LeftButton);
			QVERIFY(!image->pixmap().isNull());
			QVERIFY(image->pixmap().toImage() != sad_image);
			QVERIFY(mood->text() != sad_text);
			QVERIFY(window.windowTitle() != sad_title);

			// Стёрли всё - снова грустно
			QTest::mouseClick(find_key(&window, "Удалить"), Qt::LeftButton);
			QVERIFY(image->pixmap().toImage() == sad_image);
			QCOMPARE(mood->text(), sad_text);
			QCOMPARE(window.windowTitle(), sad_title);
		}

		void back_button_emits_signal() {
			KeyBoardWindow window;
			window.show();
			QSignalSpy spy(&window, &KeyBoardWindow::welcome_requested);

			QTest::mouseClick(window.findChild<QPushButton*>("back_button"), Qt::LeftButton);
			QCOMPARE(spy.count(), 1);
		}

		void windows_switch_via_signals_and_app_keeps_running() {
			WelcomeWindow welcome;
			KeyBoardWindow keyboard;

			QObject::connect(&welcome, &WelcomeWindow::keyboard_requested,
				&keyboard, &KeyBoardWindow::show_window);
			QObject::connect(&welcome, &WelcomeWindow::keyboard_requested,
				&welcome, &QWidget::hide);
			QObject::connect(&keyboard, &KeyBoardWindow::welcome_requested,
				&welcome, &WelcomeWindow::show_window);
			QObject::connect(&keyboard, &KeyBoardWindow::welcome_requested,
				&keyboard, &QWidget::hide);

			welcome.show();
			QVERIFY(welcome.isVisible());
			QVERIFY(!keyboard.isVisible());

			bool after_forward_ok = false;
			bool after_back_ok = false;

			QTimer::singleShot(50, [&] {
				QTest::mouseClick(welcome.findChild<QPushButton*>("to_keyboard_button"), Qt::LeftButton);
			});
			QTimer::singleShot(200, [&] {
				after_forward_ok = keyboard.isVisible() && !welcome.isVisible();
				QTest::mouseClick(keyboard.findChild<QPushButton*>("back_button"), Qt::LeftButton);
			});
			QTimer::singleShot(350, [&] {
				after_back_ok = welcome.isVisible() && !keyboard.isVisible();
				QApplication::quit();
			});

			// Если бы приложение закрылось при смене окон, exec() вернулся бы раньше
			QElapsedTimer timer;
			timer.start();
			QApplication::exec();

			QVERIFY2(timer.elapsed() >= 340, "приложение завершилось при смене окон");
			QVERIFY(after_forward_ok);
			QVERIFY(after_back_ok);
		}
};

QTEST_MAIN(TestKeyBoard)
#include "test_keyboard.moc"
