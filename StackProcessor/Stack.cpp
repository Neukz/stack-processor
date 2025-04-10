#include <iostream>
#include "Stack.h"

#pragma region Private methods
void Stack::addFirst(Element* newElement) {
    newElement->next = top;
    top = newElement;
}

void Stack::removeFirst() {
    if (top == nullptr) {
        return;
    }
    Element* newTop = top->next;
    top = newTop;
}

void Stack::handlePrint(Element* element, int index) {
    if (element == nullptr) {
        return;
    }
    handlePrint(element->next, index + 1);
    std::cout << index << ": ";
    element->list->print();
    std::cout << std::endl;
}
#pragma endregion

#pragma region Public methods
Stack::Stack() : top(nullptr) {};

void Stack::push(List* newList) {
    Element* newElement = new Element;
    newElement->list = newList;
    addFirst(newElement);
}

Element* Stack::pop() {
    if (top == nullptr) {
        return nullptr;
    }
    Element* topElement = top;
    removeFirst();
    return topElement;
}

void Stack::print() {
    handlePrint(top, 0);
}
#pragma endregion