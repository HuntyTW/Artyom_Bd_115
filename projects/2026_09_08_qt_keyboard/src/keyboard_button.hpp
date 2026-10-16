#pragma once

#include <cstddef>

#include <QPushButton>
#include <QString>
#include <QWidget>

namespace biv {
	class KeyBoardButton : public QPushButton {
		Q_OBJECT

		public:
			KeyBoardButton(const QString& text = "", QWidget* parent = nullptr);
	};
}
