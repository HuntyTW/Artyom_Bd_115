#include "menu_functions.hpp"
#include "menu_items.hpp"

#include <cstdlib>
#include <iostream>

const Artem::MenuItem* Artem::show_menu(const MenuItem* current) {
    std::cout << "Обучайка приветствует тебя, мой юный ученик!" << std::endl;
    for (int i = 1; i < current->children_count; i++) {
        std::cout << current->children[i]->title << std::endl;
    }
    std::cout << current->children[0]->title << std::endl;
    std::cout << "Обучайка > ";

    int user_input;
    std::cin >> user_input;
    std::cout << std::endl;
    
    return current->children[user_input];
}

const Artem::MenuItem* Artem::exit(const MenuItem* current) {
    std::exit(0);
}

// Функции для алгебры
const Artem::MenuItem* Artem::algebra_summ(const MenuItem* current) {
    std::cout << "Изучаем сложение!" << std::endl;
    // TODO: реализация обучения сложению
    std::cout << "Нажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
    return current->parent;
}

const Artem::MenuItem* Artem::algebra_substract(const MenuItem* current) {
    std::cout << "Изучаем вычитание!" << std::endl;
    // TODO: реализация обучения вычитанию
    std::cout << "Нажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
    return current->parent;
}

const Artem::MenuItem* Artem::algebra_multiply(const MenuItem* current) {
    std::cout << "Изучаем умножение!" << std::endl;
    // TODO: реализация обучения умножению
    std::cout << "Нажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
    return current->parent;
}

const Artem::MenuItem* Artem::algebra_divide(const MenuItem* current) {
    std::cout << "Изучаем деление!" << std::endl;
    // TODO: реализация обучения делению
    std::cout << "Нажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
    return current->parent;
}

// Функции для матанализа
const Artem::MenuItem* Artem::calculus_derivatives(const MenuItem* current) {
    std::cout << "Изучаем производные!" << std::endl;
    // TODO: реализация обучения производным
    std::cout << "Нажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
    return current->parent;
}

const Artem::MenuItem* Artem::calculus_integrals(const MenuItem* current) {
    std::cout << "Изучаем интегралы!" << std::endl;
    // TODO: реализация обучения интегралам
    std::cout << "Нажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
    return current->parent;
}

const Artem::MenuItem* Artem::calculus_limits(const MenuItem* current) {
    std::cout << "Изучаем пределы!" << std::endl;
    // TODO: реализация обучения пределам
    std::cout << "Нажмите Enter для продолжения...";
    std::cin.ignore();
    std::cin.get();
    return current->parent;
}

// Функции навигации
const Artem::MenuItem* Artem::go_back(const MenuItem* current) {
    return current->parent->parent;
}