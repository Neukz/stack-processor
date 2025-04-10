#pragma once

struct Node {
    char data;
    Node* next;
    Node* prev;
};

class List {
private:
    // Recursive handlers
    static void handleNumberConstructor(List* list, int number);
    Node* handleGetTail(Node* node);
    void handlePrint(Node* node);
    int handleGetLength(Node* node);
    void handleRemoveLast(Node* node);
    int handleToInt(Node* node, int value, int multiplier);
    List* handleCopy(Node* node);
    bool handleEquals(Node* own, Node* other);
    bool handleLessThan(Node* own, Node* other);
    List* handleAddition(List* sumList, Node* own, Node* other, int remainder);
    List* handleSubtraction(List* diffList, Node* own, Node* other);

    // Unexposed publicly
    void dropLeadingZeros(Node* node);
    void fillLeadingZeros(int number);
    bool isZero();
    bool isNegative();

public:
    Node* head;

    List();
    List(int number);

    Node* getTail();
    void print();
    int getLength();
    void addFirst(Node* newNode);
    void addLast(Node* newNode);
    void removeFirst();
    void removeLast();
    int toInt();
    List* copy();
    bool equals(List* other);
    bool lessThan(List* other);
    List* addition(List* other);
};