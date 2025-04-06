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
		getLast(head)->next = newNode;
	}

	void removeFirst() {
		if (isEmpty(head)) {
			return;
		}
		Node* newHead = head->next;
		//delete head;
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
};

struct StackElement {
	LinkedList* list;
	StackElement* next;
};

class Stack {
private:
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
		//delete top;
		top = newTop;
	}

public:
	StackElement* top;

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

	LinkedList* pop() {
		if (isEmpty(top)) {
			return nullptr;
		}
		LinkedList* returnList = top->list;
		removeFirst();
		return returnList;
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
			case ':':
				stack->push(stack->top->list);
				break;
			case ';':
			{
				LinkedList* upperList = stack->pop();
				LinkedList* lowerList = stack->pop();
				stack->push(upperList);
				stack->push(lowerList);
				break;
			}
			case '@':
				// TODO
				break;
			case '.':
				stack->top->list->addFirst(new Node{ input[inputPointer++] });
				break;
			case '>':
				cout << stack->pop()->head->data << endl;
				break;
			case '!':
			{
				LinkedList* topList = stack->pop();
				LinkedList* newTopList = new LinkedList();
				if (topList->isEmpty(topList->head) || topList->head->data == '0') {
					newTopList->addFirst(new Node{ '1' });
				} else {
					newTopList->addFirst(new Node{ '0' });
				}
				stack->push(newTopList);
				break;
			}
			case '&':
				stack->print(stack->top, 0);
				break;
			default:
				stack->top->list->addFirst(new Node{ instruction });
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