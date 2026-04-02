#include "doubly_linked_list.hpp"
#include <iostream>

using biv::DoublyLinkedList;

template<typename T>
DoublyLinkedList<T>::~DoublyLinkedList()
{
    Node* current = begin;
    while (current != nullptr)
    {
        Node* next = current->next;
        delete current;
        current = next;
    }
    begin = nullptr;
    end = nullptr;
}

template<typename T>
std::size_t DoublyLinkedList<T>::get_size() const noexcept
{
    std::size_t count = 0;
    Node* current = begin;
    while (current != nullptr)
    {
        ++count;
        current = current->next;
    }
    return count;
}

template<typename T>
bool DoublyLinkedList<T>::has_item(const T& value) const noexcept
{
    Node* current = begin;
    while (current != nullptr)
    {
        if (current->value == value)
        {
            return true;
        }
        current = current->next;
    }
    return false;
}

template<typename T>
void DoublyLinkedList<T>::print() const noexcept
{
    Node* current = begin;
    while (current != nullptr)
    {
        std::cout << current->value;
        if (current->next != nullptr)
        {
            std::cout << " ";
        }
        current = current->next;
    }
    std::cout << std::endl;
}

template<typename T>
void DoublyLinkedList<T>::push_back(const T& value)
{
    Node* new_node = new Node(value);

    if (begin == nullptr)
    {
        begin = new_node;
        end = new_node;
    }
    else
    {
        end->next = new_node;
        new_node->prev = end;
        end = new_node;
    }
}

template<typename T>
bool DoublyLinkedList<T>::remove_first(const T& value) noexcept
{
    Node* current = begin;
    while (current != nullptr)
    {
        if (current->value == value)
        {
            // Удаляем узел
            if (current->prev != nullptr)
            {
                current->prev->next = current->next;
            }
            else
            {
                // Удаляем первый элемент
                begin = current->next;
            }

            if (current->next != nullptr)
            {
                current->next->prev = current->prev;
            }
            else
            {
                // Удаляем последний элемент
                end = current->prev;
            }

            delete current;
            return true;
        }
        current = current->next;
    }
    return false;
}

template class biv::DoublyLinkedList<int>;