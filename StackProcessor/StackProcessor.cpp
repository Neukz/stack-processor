#include <iostream>
#include "StackProcessor.h"

#pragma region Private methods
List* StackProcessor::copyListAt(int index) {
    if (index == 0) {   // Found
        List* targetList = stack.pop();
        List* copiedList = targetList->copy();
        stack.push(targetList);
        return copiedList;
    }
    // Pop lists until index = 0, then push them back
    List* topList = stack.pop();
    List* targetList = copyListAt(index - 1);
    stack.push(topList);
    return targetList;
}
#pragma endregion

#pragma region Public methods
StackProcessor::StackProcessor() : programLength(0), instructionPointer(0) {}

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
                stack.push(new List());
                break;
            }
            case ',':
            {
                List* topList = stack.pop();
                delete topList;
                break;
            }
            case ':':
            {
                List* topList = stack.pop();
                List* copiedList = topList->copy();
                stack.push(topList);
                stack.push(copiedList);
                break;
            }
            case ';':
            {
                List* upperList = stack.pop();
                List* lowerList = stack.pop();
                stack.push(upperList);
                stack.push(lowerList);
                break;
            }
            case '@':
            {
                List* topList = stack.pop();
                int targetListIndex = topList->toInt();
                List* targetList = copyListAt(targetListIndex);
                stack.push(targetList);
                delete topList;
                break;
            }
            case '.':
            {
                char input;
                std::cin >> input;
                List* topList = stack.pop();
                topList->addFirst(new Node{ input });
                stack.push(topList);
                break;
            }
            case '>':
            {
                List* topList = stack.pop();
                if (topList != nullptr) {
                    std::cout << topList->head->data;
                }
                delete topList;
                break;
            }
            case '!':
            {
                List* topList = stack.pop();
                List* newTopList = new List();
                if (topList->head == nullptr || (topList->head->data == '0' && topList->head->next == nullptr)) {
                    newTopList->addFirst(new Node{ '1' });
                } else {
                    newTopList->addFirst(new Node{ '0' });
                }
                stack.push(newTopList);
                delete topList;
                break;
            }
            case '<':
            {
                List* listA = stack.pop();
                List* listB = stack.pop();
                List* newTopList = new List();
                newTopList->addFirst(new Node{ listB->lessThan(listA) ? '1' : '0' });
                stack.push(newTopList);
                delete listA;
                delete listB;
                break;
            }
            case '=':
            {
                List* listA = stack.pop();
                List* listB = stack.pop();
                List* newTopList = new List();
                newTopList->addFirst(new Node{ listB->equals(listA) ? '1' : '0' });
                stack.push(newTopList);
                delete listA;
                delete listB;
                break;
            }
            case '~':
            {
                List* newTopList = new List(instructionPointer);
                stack.push(newTopList);
                break;
            }
            case '?':
            {
                List* listT = stack.pop();
                List* listW = stack.pop();
                if (listW->head != nullptr && !(listW->head->data == '0' && listW->head->next == nullptr)) {
                    instructionPointer = listT->toInt();
                    delete listT;
                    delete listW;
                    continue;
                }
                delete listT;
                delete listW;
                break;
            }
            case '-':
            {
                List* topList = stack.pop();
                Node* lastNode = topList->getTail();
                if (lastNode != nullptr && lastNode->data == '-') {
                    topList->removeLast();
                } else {
                    topList->addLast(new Node{ '-' });
                }
                stack.push(topList);
                break;
            }
            case '^':
            {
                List* topList = stack.pop();
                Node* lastNode = topList->getTail();
                if (lastNode != nullptr && lastNode->data == '-') {
                    topList->removeLast();
                }
                stack.push(topList);
                break;
            }
            case '$':
            {
                List* topList = stack.pop();
                Node* firstNode = topList->head;
                topList->removeFirst();
                stack.push(topList);
                List* newTopList = new List();
                newTopList->addFirst(firstNode);
                stack.push(newTopList);
                break;
            }
            case '#':
            {
                List* upperList = stack.pop();
                List* lowerList = stack.pop();
                Node* lowerListLastNode = lowerList->getTail();
                lowerListLastNode->next = upperList->head;
                upperList->head->prev = lowerListLastNode;
                stack.push(lowerList);
                break;
            }
            case '+':
            {
                List* listA = stack.pop();
                List* listB = stack.pop();
                List* sumList = listA->addition(listB);
                stack.push(sumList);
                delete listA;
                delete listB;
                break;
            }
            case '&':
            {
                stack.print();
                break;
            }
            case ']':
            {
                List* topList = stack.pop();
                int ascii = topList->toInt();
                List* newTopList = new List();
                newTopList->addFirst(new Node{ (char)ascii });
                stack.push(newTopList);
                delete topList;
                break;
            }
            case '[':
            {
                List* topList = stack.pop();
                List* asciiList = new List((int)topList->head->data);
                stack.push(asciiList);
                delete topList;
                break;
            }
            default:
            {
                List* topList = stack.pop();
                if (topList != nullptr) {
                    topList->addFirst(new Node{ instruction });
                    stack.push(topList);
                }
            }
        }

        instructionPointer++;
    }
}
#pragma endregion