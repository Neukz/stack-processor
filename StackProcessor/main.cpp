#include <iostream>

using namespace std;

const int MAX_CHARS = 20000;

struct Node {
	char data;
	Node* next;
};

class LinkedList {
private:
	void print(Node* node) {
		if (node == nullptr) {
			return;
		}
		cout << node->data;
		print(node->next);
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
		if (node->next == nullptr && node->data == '-') {
			return -value;
		}
		value += (node->data - '0') * multiplier;
		return toInt(node->next, value, multiplier * 10);
	}

	LinkedList* copy(Node* node) {
		if (node == nullptr) {
			return new LinkedList();
		}
		LinkedList* copiedList = copy(node->next);
		Node* newNode = new Node{ node->data, copiedList->head };
		copiedList->head = newNode;
		return copiedList;
	}

public:
	Node* head;

	LinkedList() : head(nullptr) {};

	void print() {
		print(head);
	}

	Node* getLast() {
		return getLast(head);
	}

	void addFirst(Node* newNode) {
		newNode->next = head;
		head = newNode;
	}

	void addLast(Node* newNode) {
		newNode->next = nullptr;
		if (head == nullptr) {
			head = newNode;
			return;
		}
		Node* lastNode = getLast(head);
		lastNode->next = newNode;
	}

	void removeFirst() {
		if (head == nullptr) {
			return;
		}
		Node* newHead = head->next;
		head = newHead;
	}

	void removeLast() {
		removeLast(head);
	}

	int toInt() {
		return toInt(head, 0, 1);
	}

	LinkedList* copy() {
		return copy(head);
	}
};

struct StackElement {
	LinkedList* list;
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
		cout << index << ": ";
		element->list->print();
		cout << endl;
	}

public:
	Stack() : top(nullptr) {};

	void push(LinkedList* newList) {
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
	char program[MAX_CHARS];
	int programLength;
	char input[MAX_CHARS];
	int inputLength;
	int instructionPointer;
	int inputPointer;

	LinkedList* copyListAt(int index) {
		if (index == 0) {
			LinkedList* topList = stack->pop()->list;
			LinkedList* copiedList = topList->copy();
			stack->push(topList);
			return copiedList;
		}
		StackElement* topElement = stack->pop();
		LinkedList* targetList = copyListAt(index - 1);
		stack->push(topElement->list);
		return targetList;
	}

public:
	StackProcessor() : stack(new Stack()), programLength(0), inputLength(0), instructionPointer(0), inputPointer(0) {}

	void loadProgram() {
		while (programLength < MAX_CHARS) {
			char instruction = cin.get();
			if (instruction == '\n') {
				break;
			}
			program[programLength++] = instruction;
		}
	}

	void loadInput() {
		while (inputLength < MAX_CHARS) {
			char character = cin.get();
			if (character == '\n') {
				break;
			}
			input[inputLength++] = character;
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
				stack->push(new LinkedList());
				break;
			}
			case ',':
			{
				stack->pop();
				break;
			}
			case ':':
			{
				LinkedList* topList = stack->pop()->list;
				LinkedList* copiedList = topList->copy();
				stack->push(topList);
				stack->push(copiedList);
				break;
			}
			case ';':
			{
				LinkedList* upperList = stack->pop()->list;
				LinkedList* lowerList = stack->pop()->list;
				stack->push(upperList);
				stack->push(lowerList);
				break;
			}
			case '@':
			{
				LinkedList* topList = stack->pop()->list;
				int targetListIndex = topList->toInt();
				LinkedList* targetList = copyListAt(targetListIndex);
				stack->push(targetList);
				break;
			}
			case '.':
			{
				LinkedList* topList = stack->pop()->list;
				topList->addFirst(new Node{ input[inputPointer++] });
				stack->push(topList);
				break;
			}
			case '>':
			{
				cout << stack->pop()->list->head->data;
				break;
			}
			case '-':
			{
				LinkedList* topList = stack->pop()->list;
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
				LinkedList* topList = stack->pop()->list;
				Node* lastNode = topList->getLast();
				if (lastNode != nullptr && topList->getLast()->data == '-') {
					topList->removeLast();
				}
				stack->push(topList);
				break;
			}
			/*case '!':
			{
				LinkedList* topList = stack->pop()->list;
				LinkedList* newTopList = new LinkedList();
				if (topList->isEmpty(topList->head) || topList->head->data == '0') {
					newTopList->addFirst(new Node{ '1' });
				} else {
					newTopList->addFirst(new Node{ '0' });
				}
				stack->push(newTopList);
				break;
			}
			case '<':
			{
				LinkedList* upperList = stack->pop()->list;
				LinkedList* lowerList = stack->pop()->list;
				int a = upperList->toInt(upperList->head, 0, 1);
				int b = lowerList->toInt(lowerList->head, 0, 1);
				LinkedList* newTopList = new LinkedList();
				if (a > b) {
					newTopList->addFirst(new Node{ '1' });
				} else {
					newTopList->addFirst(new Node{ '0' });
				}
				stack->push(newTopList);
				break;
			}
			case '=':
			{
				LinkedList* upperList = stack->pop()->list;
				LinkedList* lowerList = stack->pop()->list;
				int a = upperList->toInt(upperList->head, 0, 1);
				int b = lowerList->toInt(lowerList->head, 0, 1);
				LinkedList* newTopList = new LinkedList();
				if (a == b) {
					newTopList->addFirst(new Node{ '1' });
				} else {
					newTopList->addFirst(new Node{ '0' });
				}
				stack->push(newTopList);
				break;
			}*/
			case '&':
			{
				stack->print();
				break;
			}
			default:
			{
				LinkedList* topList = stack->pop()->list;
				topList->addFirst(new Node{ instruction });
				stack->push(topList);
			}
		}

		instructionPointer++;
		execute();
	}
};

int main() {
	StackProcessor processor;
	processor.loadProgram();
	processor.loadInput();
	processor.execute();
}