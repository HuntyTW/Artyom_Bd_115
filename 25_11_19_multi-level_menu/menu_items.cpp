#include "menu_items.hpp"

#include <cstddef>

#include "menu_functions.hpp"

const Artem::MenuItem Artem::STUDY_SUMM = {
    "1 - Xoчy научиться складывать!", Artem::study_summ, &Artem::STUDY
};

const Artem::MenuItem Artem::STUDY_SUBSTRACT = {
    "2 - Xoчy научиться вычитать!", Artem::study_substract, &Artem::STUDY
};

const Artem::MenuItem Artem::STUDY_MULTIPLY = {
    "3 - Xoчy научиться умножать!", Artem::study_multiply, &Artem::STUDY
};

const Artem::MenuItem Artem::STUDY_DIVIDE = {
    "4 - Xoчy научиться делить!", Artem::study_divide, &Artem::STUDY
};

const Artem::MenuItem Artem::STUDY_GO_BACK = {
    "0 - Выйти в главное меню", Artem::study_go_back, &Artem::STUDY
};

const Artem::MenuItem Artem::STUDY_GO_TO_MATH = {
    "1 - Xoчy научиться мат.анализу!", Artem::study_substract, &Artem::STUDY
};

const Artem::MenuItem Artem::STUDY_GO_TO_ALG = {
    "2 - Xoчy научиться алгебре!", Artem::study_substract, &Artem::STUDY
};

namespace {
    const Artem::MenuItem* const study_children[] = {
		&Artem::STUDY_GO_BACK,
		&Artem::STUDY_SUMM,
		&Artem::STUDY_SUBSTRACT,
		&Artem::STUDY_MULTIPLY,
		&Artem::STUDY_DIVIDE,
		&Artem::STUDY_GO_TO_MATH,
		&Artem::STUDY_GO_TO_ALG
	};
    const int study_size = sizeof(study_children) / sizeof(study_children[0]);
}

const Artem::MenuItem Artem::STUDY = {
    "1 - Xoчy учиться математике!", Artem::show_menu, &Artem::MAIN, study_children, study_size
};

const Artem::MenuItem Artem::EXIT = {
    "0 - Я лучше пойду полежу...", Artem::exit, &Artem::MAIN
};

namespace {
    const Artem::MenuItem* const main_children[] = {
		&Artem::EXIT,
		&Artem::STUDY
	};
	const int main_size = sizeof(main_children) / sizeof(main_children[0]);
}

const Artem::MenuItem Artem::MAIN = {
	nullptr, Artem::show_menu, nullptr, main_children, main_size
};