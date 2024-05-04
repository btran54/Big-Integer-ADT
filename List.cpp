/***
* Brian Tran
* btran54
* 2024 Winter CSE101 PA6
* List.cpp
* List CPP file containing the operation of functions.
***/ 

#include <iostream>
#include <string>
#include "List.h"

// Private Contructor -----------------------------------------------------------

List::Node::Node(ListElement x) {
    data = x;
    next = nullptr;
    prev = nullptr;
}

// Class Constructors & Destructors ---------------------------------------------

// Creates new List in empty state
List::List() {
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    frontDummy->prev = NULL;
    backDummy->next = NULL;
    beforeCursor = frontDummy;
    afterCursor = backDummy;

    pos_cursor = 0;
    num_elements = 0;
}

// Copy constructor
List::List(const List& L) {
    frontDummy = new Node(0);
    backDummy = new Node(0);
    frontDummy->next = backDummy;
    backDummy->prev = frontDummy;
    frontDummy->prev = NULL;
    backDummy->next = NULL;
    beforeCursor = frontDummy;
    afterCursor = backDummy;

    pos_cursor = 0;
    num_elements = 0;

    Node* N = L.backDummy->prev;

    while (N != L.frontDummy) {
        this->insertAfter(N->data);
        N = N->prev;
    }
    frontDummy->prev = nullptr;
    backDummy->next = nullptr;
}

// Destructor

List::~List() {
    clear();
    delete(frontDummy);
    delete(backDummy);
}

// Access Functions -----------------------------------------------------------

int List::length() const {
    return num_elements;
}

ListElement List::front() const {
    if (num_elements <= 0) {
        throw std::length_error("List Error: Calling front() on empty List.\n");
    }

    return frontDummy->next->data;
}

ListElement List::back() const {
    if (num_elements <= 0) {
        throw std::length_error("List Error: Calling back() on empty List.\n");
    }

    return backDummy->prev->data;
}

int List::position() const{
    return (pos_cursor);
}

ListElement List::peekNext() const {
    if (pos_cursor >= num_elements) {
        throw std::range_error("List Error: Cannot call peekNext(). Cursor at back.\n");
    }

    return afterCursor->data;
}

ListElement List::peekPrev() const {
    if (pos_cursor < 0) {
        throw std::range_error("List Error: Cannot call peekPrev(). Cursor at front.\n");
    }

    return (beforeCursor->data);
}

// Manipulation procedures ------------------------------------------------------

void List::clear() {
    moveFront();

    while(num_elements != 0) {
        eraseAfter();
    }
}

void List::moveFront() {
    pos_cursor = 0;
    beforeCursor = frontDummy;
    afterCursor = frontDummy->next;
}

void List::moveBack() {
    pos_cursor = num_elements;
    afterCursor = backDummy;
    beforeCursor = backDummy->prev;
}

ListElement List::moveNext() {
    if (position() >= length()) {
        throw std::range_error("List Error: Cannot call moveNext(). Cursor at back.\n");
    }

    pos_cursor++;
    afterCursor = afterCursor->next;
    beforeCursor = beforeCursor->next;

    return beforeCursor->data;
}

ListElement List::movePrev() {
    if (position() <= 0) {
        throw std::range_error("List Error: Cannot call movePrev(). Cursor at front.\n");
    }

    pos_cursor--;
    beforeCursor = beforeCursor->prev;
    afterCursor = afterCursor->prev;
    
    return afterCursor->data;
}

void List::insertAfter(ListElement x) {
    Node* N = new Node(x);
    N->next = afterCursor;
    N->prev = beforeCursor;
    beforeCursor->next = N;
    afterCursor->prev = N;
    afterCursor = N;
    num_elements++;
}

void List::insertBefore(ListElement x) {
    Node* N = new Node(x);
    afterCursor->prev->next = N;
    N->prev = afterCursor->prev;
    N->next = afterCursor;
    afterCursor->prev = N;
    beforeCursor = N;
    num_elements++;
    pos_cursor++;
}

void List::setAfter(ListElement x) {
    if (pos_cursor >= num_elements) {
        throw std::range_error("List Error: Cannot call setAfter(). Cursor at back.\n");
    }

    eraseAfter();
    insertAfter(x);
}

void List::setBefore(ListElement x) {
    if (pos_cursor <= 0) {
        throw std::range_error("List Error: Cannot call setBefore(). Cursor at front.\n");
    }

    eraseBefore();
    insertBefore(x);
}

void List::eraseAfter() {
    if (pos_cursor >= num_elements) {
        throw std::range_error("List Error: Cannot call eraseAfter(). Cursor at back.\n");
    }

    Node* N = afterCursor;
    afterCursor->prev->next = afterCursor->next;
	afterCursor->next->prev = afterCursor->prev;
	beforeCursor = afterCursor->prev;
	afterCursor = beforeCursor->next;

    delete(N);
    num_elements--;
}

void List::eraseBefore() {
    if (pos_cursor <= 0) {
        throw std::range_error("List Error: Cannot call eraseBefore(). Cursor at front.\n");
    }

    Node* N = beforeCursor;
	beforeCursor->prev->next = beforeCursor->next;
	beforeCursor->next->prev = beforeCursor->prev;
	beforeCursor = afterCursor->prev;
	afterCursor = beforeCursor->next;

    delete(N);
    num_elements--;
    pos_cursor--;
}

// Other Functions ---------------------------------------------------------

int List::findNext(ListElement x) {
	Node* N = nullptr;

	for(N = afterCursor; N != backDummy; N = N->next){
		moveNext();

		if(N->data == x){
			beforeCursor = N;
			afterCursor = N->next;
			return(this->position());
		}
	}

	this->moveBack();
	return -1;
}

int List::findPrev(ListElement x) {
	Node* N = nullptr;

	for(N = beforeCursor; N != frontDummy; N = N->prev){
		movePrev();
        
		if(N->data == x){
			beforeCursor = N->prev;
			afterCursor = N;
			return(this->position());
		}
	}

	this->moveFront();
	return -1;
}

void List::cleanup() {
    Node* N = nullptr;
    Node* M = nullptr;
    Node* O = nullptr;

    for (N = frontDummy->next; N != backDummy; N = N->next) {
        int count = 0;

        for (M = N->next; M != backDummy; M = M->next) {

            if(N->data == M->data) {
                O = M;

                if (afterCursor == M) {
                    afterCursor = afterCursor->next;
                }

                else if (beforeCursor == M) {
                    beforeCursor = beforeCursor->prev;
                    pos_cursor--;
                }

                else if (beforeCursor != M && count < position()) {
                    pos_cursor--;
                }

                M->prev->next = M->next;
                M->next->prev = M->prev;
                delete(O);
                num_elements--;
            }

            count++;
        }
    }
}

List List::concat(const List& L) const {
    List C;
    Node* N = this->frontDummy->next;
    Node* M = L.frontDummy->next;

	while(N != this->backDummy){
		C.insertBefore(N->data);
		N = N->next;
	}

	while(M != L.backDummy){
		C.insertBefore(M->data);
		M = M->next;
	}

	C.moveFront();
	return C;
}

std::string List::to_string() const {
	Node* N = nullptr;
	std::string r;
	std::string p;
	std::string s = "";

	for(N = frontDummy->next; N != backDummy->prev; N = N->next){
		s += std::to_string(N->data);
	}

	p = s + std::to_string(backDummy->prev->data);
	return p;
}

bool List::equals(const List& R) const{
    bool eq = false;
    Node *N = nullptr;
    Node *M = nullptr;

    eq = (this->num_elements == R.num_elements);
    N = this->frontDummy;
    M = R.frontDummy;

    while (eq && N != backDummy) {
        eq = (N->data == M->data);
        N = N->next;
        M = M->next;
    }

    return (eq);
}

// Overriden Operators -----------------------------------------------------

std::ostream& operator<< (std::ostream& stream, const List& L) {
    return stream << L.List::to_string();
}

bool operator== (const List& A, const List& B) {
    return A.List::equals(B);
}

List& List::operator=(const List& L) {
    if (this != &L) {
        List temp = L;

		std::swap(frontDummy, temp.frontDummy);
		std::swap(backDummy, temp.backDummy);
		std::swap(afterCursor, temp.afterCursor);
		std::swap(beforeCursor, temp.beforeCursor);
		std::swap(pos_cursor, temp.pos_cursor);
		std::swap(num_elements, temp.num_elements);
    }

    return *this;
}