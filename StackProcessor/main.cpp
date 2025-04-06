#include <iostream>

const int MAX_PROGRAM_LENGTH = 20000;

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
		std::cout << node->data;
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

	LinkedList* toAsciiList(LinkedList* list, int ascii) {
		if (ascii <= 0) {
			return list;
		}
		list->addLast(new Node{ (char)('0' + ascii % 10) });
		return toAsciiList(list, ascii / 10);
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

	LinkedList* toAsciiList() {
		return toAsciiList(new LinkedList(), (int)head->data);
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
		std::cout << index << ": ";
		element->list->print();
		std::cout << std::endl;
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
	char program[MAX_PROGRAM_LENGTH];
	int programLength;
	int instructionPointer;

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
				int targetListIndex = stack->pop()->list->toInt();
				LinkedList* targetList = copyListAt(targetListIndex);
				stack->push(targetList);
				break;
			}
			case '.':
			{
				char input;
				std::cin >> input;
				LinkedList* topList = stack->pop()->list;
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
			//case '!':
			//{
			//	// TODO
			//	break;
			//}
			//case '<':
			//{
			//	// TODO
			//	break;
			//}
			//case '=':
			//{
			//	// TODO
			//	break;
			//}
			//case '~':
			//{
			//	// TODO
			//	break;
			//}
			//case '?':
			//{
			//	// TODO
			//	break;
			//}
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
			case '$':
			{
				LinkedList* topList = stack->pop()->list;
				Node* firstNode = topList->head;
				topList->removeFirst();
				stack->push(topList);
				LinkedList* newTopList = new LinkedList();
				newTopList->addFirst(firstNode);
				stack->push(newTopList);
				break;
			}
			case '#':
			{
				LinkedList* upperList = stack->pop()->list;
				LinkedList* lowerList = stack->pop()->list;
				lowerList->getLast()->next = upperList->head;
				stack->push(lowerList);
				break;
			}
			//case '+':
			//{
			//	// TODO
			//	break;
			//}
			case '&':
			{
				stack->print();
				break;
			}
			case ']':
			{
				int ascii = stack->pop()->list->toInt();
				LinkedList* newTopList = new LinkedList();
				newTopList->addFirst(new Node{ (char)ascii });
				stack->push(newTopList);
				break;
			}
			case '[':
			{
				LinkedList* asciiList = stack->pop()->list->toAsciiList();
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