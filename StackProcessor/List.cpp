#include <iostream>
#include "List.h"

#pragma region Private methods
void List::handleNumberConstructor(List* list, int number) {
    if (number <= 0) {
        if (list->head == nullptr) {    // 0 was passed
            list->addFirst(new Node{ '0' });
        }
        return;
    }
    list->addLast(new Node{ (char)('0' + number % 10) });
    handleNumberConstructor(list, number / 10);
}

Node* List::handleGetTail(Node* node) {
    if (node == nullptr || node->next == nullptr) {
        return node;
    }
    return handleGetTail(node->next);
}

void List::handlePrint(Node* node) {
    if (node == nullptr) {
        return;
    }
    std::cout << node->data;
    handlePrint(node->next);
}

int List::handleGetLength(Node* node) {
    if (node == nullptr) {
        return 0;
    }
    return 1 + handleGetLength(node->next);
}

void List::handleRemoveLast(Node* node) {
    if (node == nullptr || node->next == nullptr) {
        head = nullptr;
        return;
    }
    if (node->next->next == nullptr) {
        node->next = nullptr;
        return;
    }
    handleRemoveLast(node->next);
}

int List::handleToInt(Node* node, int value, int multiplier) {
    if (node == nullptr) {
        return value;
    }
    if (node->next == nullptr && node->data == '-') {   // Negative number
        return -value;
    }
    value += (node->data - '0') * multiplier;
    return handleToInt(node->next, value, multiplier * 10);
}

List* List::handleCopy(Node* node) {
    if (node == nullptr) {
        return new List();
    }
    List* copiedList = handleCopy(node->next);
    Node* newNode = new Node{ node->data, copiedList->head };
    if (copiedList->head != nullptr) {
        copiedList->head->prev = newNode;
    }
    copiedList->head = newNode;
    return copiedList;
}

bool List::handleEquals(Node* own, Node* other) {
    if (own == nullptr && other == nullptr) {   // Both are empty or reached the end
        return true;
    }
    if (own->data != other->data) {
        return false;
    }
    return handleEquals(own->next, other->next);
}

bool List::handleLessThan(Node* own, Node* other) {
    if (own == nullptr && other == nullptr) {   // Both are empty or reached the end
        return false;
    }
    if (own->data < other->data) {
        return true;
    }
    return handleLessThan(own->next, other->next);
}

List* List::handleAddition(List* sumList, Node* own, Node* other, int remainder) {
    if (own == nullptr && other == nullptr) {
        if (remainder > 0) {
            sumList->addLast(new Node{ (char)('0' + remainder) });
        }
        return sumList;
    }
    int sum = (own->data - '0') + (other->data - '0') + remainder;
    int digit = sum % 10;
    sumList->addLast(new Node{ (char)('0' + digit) });
    return handleAddition(sumList, own->next, other->next, sum / 10);
}

List* List::handleSubtraction(List* diffList, Node* own, Node* other) {
    if (own == nullptr && other == nullptr) {
        return diffList;
    }
    int diff = (own->data - '0') - (other->data - '0');
    if (diff < 0) { // Borrow 10 from more significant digit
        diff += 10;
        own->next->data--;
    }
    diffList->addLast(new Node{ (char)('0' + diff) });
    return handleSubtraction(diffList, own->next, other->next);
}

void List::dropLeadingZeros(Node* node) {
    if (node != nullptr) {
        if (node->data == '-') {    // Preserve '-'
            return dropLeadingZeros(node->prev);
        }
        if (node->prev == nullptr || node->data != '0') {
            return;
        }
        // Drop the node with 0
        node->prev->next = node->next;
        if (node->next != nullptr) {
            node->next->prev = node->prev;
        }
        dropLeadingZeros(node->prev);
    }
}

void List::fillLeadingZeros(int number) {
    if (number == 0) {
        return;
    }
    addLast(new Node{ '0' });
    fillLeadingZeros(number - 1);
}

bool List::isZero() {
    if (head != nullptr) {
        // 0 or 0-
        return (head->data == '0' && head->next == nullptr)
            || (head->data == '0' && head->next->data == '-' && head->next->next == nullptr);
    }
    return false;
}

bool List::isNegative() {
    return getTail()->data == '-';
}
#pragma endregion

#pragma region Public methods
List::List() : head(nullptr) {}

List::List(int number) : head(nullptr) {
    handleNumberConstructor(this, number);
}

Node* List::getTail() {
    return handleGetTail(head);
}

void List::print() {
    handlePrint(head);
}

int List::getLength() {
    return handleGetLength(head);
}

void List::addFirst(Node* newNode) {
    newNode->prev = nullptr;
    newNode->next = head;
    if (head != nullptr) {
        head->prev = newNode;
    }
    head = newNode;
}

void List::addLast(Node* newNode) {
    newNode->next = nullptr;
    if (head == nullptr) {
        newNode->prev = nullptr;
        head = newNode;
        return;
    }
    Node* lastNode = getTail();
    newNode->prev = lastNode;
    lastNode->next = newNode;
}

void List::removeFirst() {
    if (head == nullptr) {
        return;
    }
    Node* newHead = head->next;
    if (newHead != nullptr) {
        newHead->prev = nullptr;
    }
    head = newHead;
}

void List::removeLast() {
    handleRemoveLast(head);
}

int List::toInt() {
    return handleToInt(head, 0, 1);
}

List* List::copy() {
    return handleCopy(head);
}

bool List::equals(List* other) {
    List* thisCopy = this->copy();
    thisCopy->dropLeadingZeros(thisCopy->getTail());
    int thisLength = thisCopy->getLength();

    List* otherCopy = other->copy();
    thisCopy->dropLeadingZeros(otherCopy->getTail());
    int otherLength = otherCopy->getLength();

    if (thisCopy->isZero() && otherCopy->isZero()) {    // Check for 0 = 0-
        return true;
    }
    if (thisLength == otherLength) {
        return handleEquals(thisCopy->head, otherCopy->head);
    }
    return false;
}

bool List::lessThan(List* other) {
    List* thisCopy = this->copy();
    thisCopy->dropLeadingZeros(thisCopy->getTail());
    int thisLength = thisCopy->getLength();
    bool thisNegative = thisCopy->isNegative();

    List* otherCopy = other->copy();
    thisCopy->dropLeadingZeros(otherCopy->getTail());
    int otherLength = otherCopy->getLength();
    bool otherNegative = otherCopy->isNegative();

    if (thisCopy->isZero() && otherCopy->isZero()) {
        return false;
    }
    if (!thisNegative && otherNegative) {
        return false;
    }
    if (thisNegative && !otherNegative) {
        return true;
    }
    if (thisLength != otherLength) {
        if (thisNegative && otherNegative) {
            return thisLength > otherLength;    // Negative with bigger length is smaller
        }
        return thisLength < otherLength;    // Positive with smaller length is smaller
    }
    return handleLessThan(thisCopy->head, otherCopy->head);
}

List* List::addition(List* other) {
    List* thisCopy = this->copy();
    bool thisNegative = thisCopy->isNegative();
    bool thisNegated = false;

    List* otherCopy = other->copy();
    bool otherNegative = otherCopy->isNegative();
    bool otherNegated = false;

    // Ignore negative signs
    if (thisNegative) {
        thisCopy->removeLast();
        thisNegated = true;
    }
    if (otherNegative) {
        otherCopy->removeLast();
        otherNegated = true;
    }

    // Fill the shorter number with leading zeros
    int lengthDiff = thisCopy->getLength() - otherCopy->getLength();
    if (lengthDiff > 0) {
        otherCopy->fillLeadingZeros(lengthDiff);
    } else if (lengthDiff < 0) {
        thisCopy->fillLeadingZeros(0 - lengthDiff);
    }

    // For numbers of the same sign - perform addition
    List* sumList = new List();
    if (!thisNegative && !otherNegative) {
        sumList = handleAddition(sumList, thisCopy->head, otherCopy->head, 0);
    } else if (thisNegative && otherNegative) {
        sumList = handleAddition(sumList, thisCopy->head, otherCopy->head, 0);
        sumList->addLast(new Node{ '-' }); // Restore '-'

        // For numbers of opposite signs - perform subtraction
        // Always subtract the smaller absolute value from the bigger one
    } else if (thisCopy->lessThan(otherCopy)) {
        sumList = handleSubtraction(sumList, otherCopy->head, thisCopy->head);
        if (otherNegated) {
            sumList->addLast(new Node{ '-' });
        }
    } else {
        sumList = handleSubtraction(sumList, thisCopy->head, otherCopy->head);
        if (thisNegated) {
            sumList->addLast(new Node{ '-' });
        }
    }

    sumList->dropLeadingZeros(sumList->getTail());
    // Format 0- to 0
    if (sumList->isZero() && sumList->isNegative()) {
        sumList->removeLast();
    }
    return sumList;
}
#pragma endregion