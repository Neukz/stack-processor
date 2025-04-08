#include <iostream>

const int MAX_PROGRAM_LENGTH = 20000;

struct Node {
    char data;
    Node* next;
    Node* prev;
};

class List {
private:
    static List* fromNumber(List* list, int number) {
        if (number <= 0) {
            if (list->head == nullptr) {    // 0 was passed
                list->addFirst(new Node{ '0' });
            }
            return list;
        }
        list->addLast(new Node{ (char)('0' + number % 10) });
        return fromNumber(list, number / 10);
    }

    void print(Node* node) {
        if (node == nullptr) {
            return;
        }
        std::cout << node->data;
        print(node->next);
    }
    
    int getLength(Node* node) {
        if (node == nullptr) {
            return 0;
        }
        return 1 + getLength(node->next);
    }

    Node* getLast(Node* node) {
        if (node == nullptr || node->next == nullptr) {
            return node;
        }
        return getLast(node->next);
    }

    void removeLast(Node* node) {
        if (node == nullptr || node->next == nullptr) {
            head = nullptr;
            return;
        }
        if (node->next->next == nullptr) {
            node->next = nullptr;
            return;
        }
        removeLast(node->next);
    }

    int toInt(Node* node, int value, int multiplier) {
        if (node == nullptr) {
            return value;
        }
        if (node->next == nullptr && node->data == '-') {   // Negative number
            return -value;
        }
        value += (node->data - '0') * multiplier;
        return toInt(node->next, value, multiplier * 10);
    }

    List* copy(Node* node) {
        if (node == nullptr) {
            return new List();
        }
        List* copiedList = copy(node->next);
        Node* newNode = new Node{ node->data, copiedList->head };
        if (copiedList->head != nullptr) {
            copiedList->head->prev = newNode;
        }
        copiedList->head = newNode;
        return copiedList;
    }

    void dropLeadingZeros(Node* node) {
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

    bool isZero() {
        // 0 or 0-
        return (head->data == '0' && head->next == nullptr)
            || (head->data == '0' && head->next->data == '-' && head->next->next == nullptr);
    }

    bool isNegative() {
        return getLast()->data == '-';
    }

    bool equals(Node* own, Node* other) {
        if (own == nullptr && other == nullptr) {   // Both are empty or reached the end
            return true;
        }
        if (own->data != other->data) {
            return false;
        }
        return equals(own->next, other->next);
    }

    bool lessThan(Node* own, Node* other) {
        if (own == nullptr && other == nullptr) {   // Both are empty or reached the end
            return false;
        }
        if (own->data < other->data) {
            return true;
        }
        return lessThan(own->next, other->next);
    }

public:
    Node* head;

    List() : head(nullptr) {};

    static List* fromNumber(int number) {
        return fromNumber(new List(), number);
    }

    void print() {
        print(head);
    }

    int getLength() {
        return getLength(head);
    }

    Node* getLast() {
        return getLast(head);
    }

    void addFirst(Node* newNode) {
        newNode->prev = nullptr;
        newNode->next = head;
        if (head != nullptr) {
            head->prev = newNode;
        }
        head = newNode;
    }

    void addLast(Node* newNode) {
        newNode->next = nullptr;
        if (head == nullptr) {
            newNode->prev = nullptr;
            head = newNode;
            return;
        }
        Node* lastNode = getLast();
        newNode->prev = lastNode;
        lastNode->next = newNode;
    }

    void removeFirst() {
        if (head == nullptr) {
            return;
        }
        Node* newHead = head->next;
        if (newHead != nullptr) {
            newHead->prev = nullptr;
        }
        head = newHead;
    }

    void removeLast() {
        removeLast(head);
    }

    int toInt() {
        return toInt(head, 0, 1);
    }

    List* copy() {
        return copy(head);
    }

    bool equals(List* other) {
        List* thisCopy = this->copy();
        thisCopy->dropLeadingZeros(thisCopy->getLast());
        int thisLength = thisCopy->getLength();

        List* otherCopy = other->copy();
        thisCopy->dropLeadingZeros(otherCopy->getLast());
        int otherLength = otherCopy->getLength();

        if (thisCopy->isZero() && otherCopy->isZero()) {    // Check for 0 = 0-
            return true;
        }
        if (thisLength == otherLength) {
            return equals(thisCopy->head, otherCopy->head);
        }
        return false;
    }

    bool lessThan(List* other) {
        List* thisCopy = this->copy();
        thisCopy->dropLeadingZeros(thisCopy->getLast());
        int thisLength = thisCopy->getLength();
        int thisNegative = thisCopy->isNegative();

        List* otherCopy = other->copy();
        thisCopy->dropLeadingZeros(otherCopy->getLast());
        int otherLength = otherCopy->getLength();
        int otherNegative = otherCopy->isNegative();

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
        return lessThan(thisCopy->head, otherCopy->head);
    }
};

struct StackElement {
    List* list;
    StackElement* next;
};

class Stack {
private:
    StackElement* top;

    void addFirst(StackElement* newElement) {
        newElement->next = top;
        top = newElement;
    }

    void removeFirst() {
        if (top == nullptr) {
            return;
        }
        StackElement* newTop = top->next;
        top = newTop;
    }

    void print(StackElement* element, int index) {
        if (element == nullptr) {
            return;
        }
        print(element->next, index + 1);
        std::cout << index << ": ";
        element->list->print();
        std::cout << std::endl;
    }

public:
    Stack() : top(nullptr) {};

    void push(List* newList) {
        StackElement* newElement = new StackElement;
        newElement->list = newList;
        addFirst(newElement);
    }

    StackElement* pop() {
        if (top == nullptr) {
            return nullptr;
        }
        StackElement* topElement = top;
        removeFirst();
        return topElement;
    }

    void print() {
        print(top, 0);
    }
};

class StackProcessor {
private:
    Stack* stack;
    char program[MAX_PROGRAM_LENGTH];
    int programLength;
    int instructionPointer;

    List* copyListAt(int index) {
        if (index == 0) {   // Found
            List* targetList = stack->pop()->list;
            List* copiedList = targetList->copy();
            stack->push(targetList);
            return copiedList;
        }
        // Pop lists until index = 0, then push them back
        StackElement* topElement = stack->pop();
        List* targetList = copyListAt(index - 1);
        stack->push(topElement->list);
        return targetList;
    }

public:
    StackProcessor() : stack(new Stack()), programLength(0), instructionPointer(0) {}

    void loadProgram() {
        while (programLength < MAX_PROGRAM_LENGTH) {
            char instruction = std::cin.get();
            if (instruction == '\n') {
                break;
            }
            program[programLength++] = instruction;
        }
    }

    void execute() {
        if (instructionPointer >= programLength) {
            return;
        }

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
                StackElement* topElement = stack->pop();
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
                    newTopList->addFirst(new Node{ listB->lessThan(listA) ? '1' : '0'});
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
                List* newTopList = List::fromNumber(instructionPointer);
                stack->push(newTopList);
                break;
            }
            case '?':
            {
                    int t = stack->pop()->list->toInt();
                    List* listW = stack->pop()->list;
                    if (listW->head != nullptr && !(listW->head->data == '0' && listW->head->next == nullptr)) {
                            instructionPointer = t;
                            return execute();
                    }
                    break;
            }
            case '-':
            {
                List* topList = stack->pop()->list;
                Node* lastNode = topList->getLast();
                if (lastNode != nullptr && topList->getLast()->data == '-') {
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
                Node* lastNode = topList->getLast();
                if (lastNode != nullptr && topList->getLast()->data == '-') {
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
                Node* lowerListLastNode = lowerList->getLast();
                lowerListLastNode->next = upperList->head;
                upperList->head->prev = lowerListLastNode;
                stack->push(lowerList);
                break;
            }
            //case '+':
            //{
            //      // TODO
            //      break;
            //}
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
                List* asciiList = List::fromNumber((int)stack->pop()->list->head->data);
                stack->push(asciiList);
                break;
            }
            default:
            {
                StackElement* topElement = stack->pop();
                if (topElement != nullptr) {
                    topElement->list->addFirst(new Node{ instruction });
                    stack->push(topElement->list);
                }
            }
        }

        instructionPointer++;
        execute();
    }
};

int main() {
    StackProcessor processor;
    processor.loadProgram();
    processor.execute();
}