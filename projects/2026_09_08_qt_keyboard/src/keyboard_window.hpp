#pragma once

#include <cstddef>

#include <QKeyEvent>
#include <QLabel>
#include <QPixmap>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QWidget>

#include "keyboard.hpp"

namespace biv {
	class KeyBoardWindow : public QWidget {
		Q_OBJECT

		private:
			QLabel* image;
			QLabel* mood_label;
			QPlainTextEdit* display;
			KeyBoard* keyboard;

			QPixmap sad_pixmap;
			QPixmap happy_pixmap;

		public:
			KeyBoardWindow(QWidget* parent = nullptr);

		public slots:
			void show_window();

			void add_symbol(const QString& text);
			void add_space();
			void add_enter();
			void remove_symbol();

		signals:
			// Пользователь нажал кнопку "Назад"
			void welcome_requested();

		protected:
			void keyPressEvent(QKeyEvent* event) override;

		private:
			int get_key_code(const QKeyEvent* event) const;
			void update_mood();
	};
}
