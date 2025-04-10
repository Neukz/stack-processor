#include <iostream>
#include "StackProcessor.h"

#pragma region Private methods
List* StackProcessor::copyListAt(int index) {
    if (index == 0) {   // Found
        List* targetList = stack->pop()->list;
        List* copiedList = targetList->copy();
        stack->push(targetList);
        return copiedList;
    }
    // Pop lists until index = 0, then push them back
    Element* topElement = stack->pop();
    List* targetList = copyListAt(index - 1);
    stack->push(topElement->list);
    return targetList;
}
#pragma endregion

#pragma region Public methods
StackProcessor::StackProcessor() : stack(new Stack()), programLength(0), instructionPointer(0) {}

void StackProcessor::loadProgram() {
    while (programLength < MAX_PROGRAM_LENGTH) {
        char instruction = std::cin.get();
        if (instruction == '\n') {
            break;
        }
        program[programLength++] = instruction;
    }
}

void StackProcessor::execute() {
    while (instructionPointer < programLength) {
        char instruction = program[instructionPointer];
        switch (instruction) {
            case '\'':
            {
                stack->push(new List());
                break;
            }
            case ',':
            {
                stack->pop();
                break;
            }
            case ':':
            {
                List* topList = stack->pop()->list;
                List* copiedList = topList->copy();
                stack->push(topList);
                stack->push(copiedList);
                break;
            }
            case ';':
            {
                List* upperList = stack->pop()->list;
                List* lowerList = stack->pop()->list;
                stack->push(upperList);
                stack->push(lowerList);
                break;
            }
            case '@':
            {
                int targetListIndex = stack->pop()->list->toInt();
                List* targetList = copyListAt(targetListIndex);
                stack->push(targetList);
                break;
            }
            case '.':
            {
                char input;
                std::cin >> input;
                List* topList = stack->pop()->list;
                topList->addFirst(new Node{ input });
                stack->push(topList);
                break;
            }
            case '>':
            {
                Element* topElement = stack->pop();
                if (topElement != nullptr) {
                    std::cout << topElement->list->head->data;
                }
                break;
            }
            case '!':
            {
                List* topList = stack->pop()->list;
                List* newTopList = new List();
                if (topList->head == nullptr || (topList->head->data == '0' && topList->head->next == nullptr)) {
                    newTopList->addFirst(new Node{ '1' });
                } else {
                    newTopList->addFirst(new Node{ '0' });
                }
                stack->push(newTopList);
                break;
            }
            case '<':
            {
                List* listA = stack->pop()->list;
                List* listB = stack->pop()->list;
                List* newTopList = new List();
                newTopList->addFirst(new Node{ listB->lessThan(listA) ? '1' : '0' });
                stack->push(newTopList);
                break;
            }
            case '=':
            {
                List* listA = stack->pop()->list;
                List* listB = stack->pop()->list;
                List* newTopList = new List();
                newTopList->addFirst(new Node{ listB->equals(listA) ? '1' : '0' });
                stack->push(newTopList);
                break;
            }
            case '~':
            {
                List* newTopList = new List(instructionPointer);
                stack->push(newTopList);
                break;
            }
            case '?':
            {
                int t = stack->pop()->list->toInt();
                List* listW = stack->pop()->list;
                if (listW->head != nullptr && !(listW->head->data == '0' && listW->head->next == nullptr)) {
                    instructionPointer = t;
                    continue;
                }
                break;
            }
            case '-':
            {
                List* topList = stack->pop()->list;
                Node* lastNode = topList->getTail();
                if (lastNode != nullptr && topList->getTail()->data == '-') {
                    topList->removeLast();
                } else {
                    topList->addLast(new Node{ '-' });
                }
                stack->push(topList);
                break;
            }
            case '^':
            {
                List* topList = stack->pop()->list;
                Node* lastNode = topList->getTail();
                if (lastNode != nullptr && topList->getTail()->data == '-') {
                    topList->removeLast();
                }
                stack->push(topList);
                break;
            }
            case '$':
            {
                List* topList = stack->pop()->list;
                Node* firstNode = topList->head;
                topList->removeFirst();
                stack->push(topList);
                List* newTopList = new List();
                newTopList->addFirst(firstNode);
                stack->push(newTopList);
                break;
            }
            case '#':
            {
                List* upperList = stack->pop()->list;
                List* lowerList = stack->pop()->list;
                Node* lowerListLastNode = lowerList->getTail();
                lowerListLastNode->next = upperList->head;
                upperList->head->prev = lowerListLastNode;
                stack->push(lowerList);
                break;
            }
            case '+':
            {
                List* listA = stack->pop()->list;
                List* listB = stack->pop()->list;
                List* sumList = listA->addition(listB);
                stack->push(sumList);
                break;
            }
            case '&':
            {
                stack->print();
                break;
            }
            case ']':
            {
                int ascii = stack->pop()->list->toInt();
                List* newTopList = new List();
                newTopList->addFirst(new Node{ (char)ascii });
                stack->push(newTopList);
                break;
            }
            case '[':
            {
                List* asciiList = new List((int)stack->pop()->list->head->data);
                stack->push(asciiList);
                break;
            }
            default:
            {
                Element* topElement = stack->pop();
                if (topElement != nullptr) {
                    topElement->list->addFirst(new Node{ instruction });
                    stack->push(topElement->list);
                }
            }
        }

        instructionPointer++;
    }
}
#pragma endregion