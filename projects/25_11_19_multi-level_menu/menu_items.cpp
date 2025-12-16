#include "menu_items.hpp"

#include <cstddef>

#include "menu_functions.hpp"

// Уровень 3: Операции алгебры
const Artem::MenuItem Artem::ALGEBRA_SUMM = {
    "1 - Изучить сложение", Artem::algebra_summ, &Artem::STUDY_ALGEBRA
};

const Artem::MenuItem Artem::ALGEBRA_SUBSTRACT = {
    "2 - Изучить вычитание", Artem::algebra_substract, &Artem::STUDY_ALGEBRA
};

const Artem::MenuItem Artem::ALGEBRA_MULTIPLY = {
    "3 - Изучить умножение", Artem::algebra_multiply, &Artem::STUDY_ALGEBRA
};

const Artem::MenuItem Artem::ALGEBRA_DIVIDE = {
    "4 - Изучить деление", Artem::algebra_divide, &Artem::STUDY_ALGEBRA
};

const Artem::MenuItem Artem::ALGEBRA_GO_BACK = {
    "0 - Вернуться к выбору раздела", Artem::go_back, &Artem::STUDY_ALGEBRA
};

// Уровень 3: Операции матанализа
const Artem::MenuItem Artem::CALCULUS_DERIVATIVES = {
    "1 - Изучить производные", Artem::calculus_derivatives, &Artem::STUDY_CALCULUS
};

const Artem::MenuItem Artem::CALCULUS_INTEGRALS = {
    "2 - Изучить интегралы", Artem::calculus_integrals, &Artem::STUDY_CALCULUS
};

const Artem::MenuItem Artem::CALCULUS_LIMITS = {
    "3 - Изучить пределы", Artem::calculus_limits, &Artem::STUDY_CALCULUS
};

const Artem::MenuItem Artem::CALCULUS_GO_BACK = {
    "0 - Вернуться к выбору раздела", Artem::go_back, &Artem::STUDY_CALCULUS
};

// Уровень 2: Выбор раздела математики
const Artem::MenuItem Artem::STUDY_GO_BACK = {
    "0 - Вернуться в главное меню", Artem::go_back, &Artem::STUDY
};

// Дочерние элементы для меню алгебры
namespace {
    const Artem::MenuItem* const algebra_children[] = {
        &Artem::ALGEBRA_GO_BACK,
        &Artem::ALGEBRA_SUMM,
        &Artem::ALGEBRA_SUBSTRACT,
        &Artem::ALGEBRA_MULTIPLY,
        &Artem::ALGEBRA_DIVIDE
    };
    const int algebra_size = sizeof(algebra_children) / sizeof(algebra_children[0]);
}

// Дочерние элементы для меню матанализа
namespace {
    const Artem::MenuItem* const calculus_children[] = {
        &Artem::CALCULUS_GO_BACK,
        &Artem::CALCULUS_DERIVATIVES,
        &Artem::CALCULUS_INTEGRALS,
        &Artem::CALCULUS_LIMITS
    };
    const int calculus_size = sizeof(calculus_children) / sizeof(calculus_children[0]);
}

// Дочерние элементы для меню выбора раздела
namespace {
    const Artem::MenuItem* const study_children[] = {
        &Artem::STUDY_GO_BACK,
        &Artem::STUDY_ALGEBRA,
        &Artem::STUDY_CALCULUS
    };
    const int study_size = sizeof(study_children) / sizeof(study_children[0]);
}

// Дочерние элементы для главного меню
namespace {
    const Artem::MenuItem* const main_children[] = {
        &Artem::EXIT,
        &Artem::STUDY
    };
    const int main_size = sizeof(main_children) / sizeof(main_children[0]);
}

// Инициализация меню с дочерними элементами (должна быть после объявления массивов)
const Artem::MenuItem Artem::STUDY_ALGEBRA = {
    "1 - Хочу учиться алгебре!", Artem::show_menu, &Artem::STUDY, algebra_children, algebra_size
};

const Artem::MenuItem Artem::STUDY_CALCULUS = {
    "2 - Хочу учиться матанализу!", Artem::show_menu, &Artem::STUDY, calculus_children, calculus_size
};

const Artem::MenuItem Artem::STUDY = {
    "1 - Хочу учиться математике!", Artem::show_menu, &Artem::MAIN, study_children, study_size
};

const Artem::MenuItem Artem::EXIT = {
    "0 - Я лучше пойду полежу...", Artem::exit, &Artem::MAIN
};

const Artem::MenuItem Artem::MAIN = {
    nullptr, Artem::show_menu, nullptr, main_children, main_size
};