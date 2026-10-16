#pragma once

#include <cstddef>

#include <QWidget>

namespace biv {
	class WelcomeWindow : public QWidget {
		Q_OBJECT

		public:
			WelcomeWindow(QWidget* parent = nullptr);

		public slots:
			void show_window();

		signals:
			// Пользователь нажал кнопку "Перейти к клавиатуре"
			void keyboard_requested();
	};
}
