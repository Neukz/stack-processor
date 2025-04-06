#include <iostream>

using namespace std;

const int MAX_CHARS = 20000;

struct Node {
	char data;
	Node* next;
};

class LinkedList {
public:
	Node* head;

	LinkedList() : head(nullptr) {};

	bool isEmpty(Node* node) {
		return node == nullptr;
	}

	void print(Node* node) {
		if (isEmpty(node)) {
			return;
		}
		cout << node->data;
		print(node->next);
	}

	Node* getLast(Node* node) {
		if (isEmpty(node) || isEmpty(node->next)) {
			return node;
		}
		return getLast(node->next);
	}

	void addFirst(Node* newNode) {
		newNode->next = head;
		head = newNode;
	}

	void addLast(Node* newNode) {
		newNode->next = nullptr;
		if (isEmpty(head)) {
			head = newNode;
			return;
		}
		Node* lastNode = getLast(head);
		lastNode->next = newNode;
	}

	void removeFirst() {
		if (isEmpty(head)) {
			return;
		}
		Node* newHead = head->next;
		head = newHead;
	}

	void removeLast(Node* node) {
		if (isEmpty(node)) {
			return;
		}
		if (isEmpty(node->next)) {
			delete node;
			return;
		}
		if (isEmpty(node->next->next)) {
			delete node->next;
			node->next = nullptr;
			return;
		}
		removeLast(node->next);
	}

	int toInt(Node* node, int value, int multiplier) {
		if (isEmpty(node)) {
			return value;
		}
		if (node->next == nullptr && node->data == '-') {
			return -value;
		}
		value += (node->data - '0') * multiplier;
		return toInt(node->next, value, multiplier * 10);
	}

	LinkedList* copy(Node* node) {
		if (isEmpty(node)) {
			return new LinkedList();
		}
		LinkedList* copiedList = copy(node->next);
		Node* newNode = new Node{ node->data, copiedList->head };
		copiedList->head = newNode;
		return copiedList;
	}
};

struct StackElement {
	LinkedList* list;
	StackElement* next;
};

class Stack {
private:
	StackElement* top;

	bool isEmpty(StackElement* element) {
		return element == nullptr;
	}

	void addFirst(StackElement* newElement) {
		newElement->next = top;
		top = newElement;
	}

	void removeFirst() {
		if (isEmpty(top)) {
			return;
		}
		StackElement* newTop = top->next;
		top = newTop;
	}

public:
	Stack() : top(nullptr) {};

	void print(StackElement* element, int index) {
		if (isEmpty(element)) {
			return;
		}
		print(element->next, index + 1);
		cout << index << ": ";
		element->list->print(element->list->head);
		cout << endl;
	}

	void push(LinkedList* newList) {
		StackElement* newElement = new StackElement;
		newElement->list = newList;
		addFirst(newElement);
	}

	StackElement* pop() {
		if (isEmpty(top)) {
			return nullptr;
		}
		StackElement* topElement = top;
		removeFirst();
		return topElement;
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
			LinkedList* copiedList = topList->copy(topList->head);
			stack->push(topList);
			return copiedList;
		}
		StackElement* topElement = stack->pop();
		LinkedList* targetList = copyListAt(index - 1);
		stack->push(topElement->list);
		return targetList;
	}

public:
	StackProcessor() : stack(new Stack()), programLength(0), inputLength(0), instructionPointer(0) {}

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
				stack->push(new LinkedList());
				break;
			case ',':
				stack->pop();
				break;
			case ':':
			{
				LinkedList* topList = stack->pop()->list;
				LinkedList* copiedList = topList->copy(topList->head);
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
				int targetListIndex = topList->toInt(topList->head, 0, 1);
				LinkedList* targetList = copyListAt(targetListIndex);
				stack->push(targetList);
				break;
			}
			/*case '.':
			{
				LinkedList* topList = stack->pop()->list;
				topList->addFirst(new Node{ input[inputPointer++] });
				stack->push(topList);
				break;
			}
			case '>':
				cout << stack->pop()->list->head->data << endl;
				break;
			case '!':
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
			}*/
			case '&':
			{
				StackElement* topElement = stack->pop();
				stack->push(topElement->list);
				stack->print(topElement, 0);
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