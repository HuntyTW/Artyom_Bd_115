#pragma once

#include "menu.hpp"

namespace Artem {
	const MenuItem* show_menu(const MenuItem* current);
	
	const MenuItem* exit(const MenuItem* current);
	
	// Функции для алгебры
	const MenuItem* algebra_summ(const MenuItem* current);
	const MenuItem* algebra_substract(const MenuItem* current);
	const MenuItem* algebra_multiply(const MenuItem* current);
	const MenuItem* algebra_divide(const MenuItem* current);
	
	// Функции для матанализа
	const MenuItem* calculus_derivatives(const MenuItem* current);
	const MenuItem* calculus_integrals(const MenuItem* current);
	const MenuItem* calculus_limits(const MenuItem* current);
	
	// Функции навигации
	const MenuItem* go_back(const MenuItem* current);
}