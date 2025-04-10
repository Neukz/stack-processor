#pragma once
#include "List.h"

struct Element {
    List* list;
    Element* next;
};

class Stack {
private:
    Element* top;

    void addFirst(Element* newElement);
    void removeFirst();
    void handlePrint(Element* element, int index);

public:
    Stack();

    void push(List* newList);
    List* pop();
    void print();
};