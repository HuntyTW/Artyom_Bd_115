#include "vector.hpp"
#include <iostream>

using biv::Vector;

// Определяем статическую константу (начальная вместимость = 4)
template<typename T>
const std::size_t Vector<T>::START_CAPACITY = 4;

template<typename T>
Vector<T>::Vector() : arr(new T[START_CAPACITY]) {
    // capacity и size уже инициализированы в классе
}

template<typename T>
Vector<T>::~Vector() {
    delete[] arr;
}

template<typename T>
std::size_t Vector<T>::get_size() const noexcept {
    return size;
}

template<typename T>
bool Vector<T>::has_item(const T& value) const noexcept {
    for (std::size_t i = 0; i < size; ++i) {
        if (arr[i] == value) {
            return true;
        }
    }
    return false;
}

template<typename T>
bool Vector<T>::insert(const std::size_t position, const T& value) {
    if (position > size) {
        return false; // позиция недопустима
    }

    // Если нужно, увеличиваем вместимость (в 2 раза)
    if (size == capacity) {
        std::size_t new_capacity = capacity * 2;
        T* new_arr = new T[new_capacity];

        // Копируем существующие элементы
        for (std::size_t i = 0; i < size; ++i) {
            new_arr[i] = arr[i];
        }

        delete[] arr;
        arr = new_arr;
        capacity = new_capacity;
    }

    // Сдвигаем элементы вправо, освобождая место
    for (std::size_t i = size; i > position; --i) {
        arr[i] = arr[i - 1];
    }

    arr[position] = value;
    ++size;
    return true;
}

template<typename T>
void Vector<T>::print() const noexcept {
    for (std::size_t i = 0; i < size; ++i) {
        if (i > 0) {
            std::cout << " ";
        }
        std::cout << arr[i];
    }
    std::cout << std::endl;
}

template<typename T>
void Vector<T>::push_back(const T& value) {
    // Если нужно, увеличиваем вместимость (в 2 раза)
    if (size == capacity) {
        std::size_t new_capacity = capacity * 2;
        T* new_arr = new T[new_capacity];

        // Копируем существующие элементы
        for (std::size_t i = 0; i < size; ++i) {
            new_arr[i] = arr[i];
        }

        delete[] arr;
        arr = new_arr;
        capacity = new_capacity;
    }

    arr[size] = value;
    ++size;
}

template<typename T>
bool Vector<T>::remove_first(const T& value) {
    for (std::size_t i = 0; i < size; ++i) {
        if (arr[i] == value) {
            // Сдвигаем все последующие элементы влево
            for (std::size_t j = i; j < size - 1; ++j) {
                arr[j] = arr[j + 1];
            }
            --size;
            return true;
        }
    }
    return false; // элемент не найден
}