#include "menu_functions.hpp"

#include <cstdlib>
#include <iostream>

const Artem::MenuItem* Artem::show_menu(const MenuItem* current) {
    std::cout << "Обучайка npuserствует тебя, мой юный ученик!" << std::endl;
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

const Artem::MenuItem* Artem::study_summ(const MenuItem* current) {
	//TODO
	std::cout << current->title << std::endl;
	return current->parent;
}

const Artem::MenuItem* Artem::study_substract(const MenuItem* current) {
	//TODO
	std::cout << current->title << std::endl;
	return current->parent;
}

const Artem::MenuItem* Artem::study_multiply(const MenuItem* current) {
	//TODO
	std::cout << current->title << std::endl;
	return current->parent;
}

const Artem::MenuItem* Artem::study_divide(const MenuItem* current) {
	//TODO
	std::cout << current->title << std::endl;
	return current->parent;
}

const Artem::MenuItem* Artem::study_go_back(const MenuItem* current) {
	//TODO
	std::cout << current->title << std::endl;
	return current->parent->parent;
}

const Artem::MenuItem* Artem::study_go_to_math(const MenuItem* current) {
	//TODO
	std::cout << current->title << std::endl;
	return current->parent->parent;
}

const Artem::MenuItem* Artem::study_go_to_alg(const MenuItem* current) {
	//TODO
	std::cout << current->title << std::endl;
	return current->parent->parent;
}