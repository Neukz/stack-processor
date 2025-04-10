#pragma once
#include "Stack.h"

class StackProcessor {
private:
    static const int MAX_PROGRAM_LENGTH = 20000;

    Stack stack;
    char program[MAX_PROGRAM_LENGTH];
    int programLength;
    int instructionPointer;

    List* copyListAt(int index);

public:
    StackProcessor();

    void loadProgram();
    void execute();
};