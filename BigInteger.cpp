/***
* Brian Tran
* btran54
* 2024 Winter CSE101 PA6
* BigInteger.cpp
* BigInteger CPP file containing the operation of functions.
***/

#include <iostream>
#include <string>
#include <stdexcept>
#include <cstring>

#include "BigInteger.h"
#include "List.h"

const int ASCIIzero = 48;

BigInteger::BigInteger() {
    signum = 0;
}

BigInteger::BigInteger(long x) {
    signum = (x == 0) ? 0 : (x > 0) ? 1 : -1;
    x = std::abs(x);

	if (x == 0) {
		digits.insertBefore(0);
		return;
	}

    while (x > 0) {
        digits.insertBefore(x % base);
        x /= base;
    }
}

BigInteger::BigInteger(std::string s){
	// Get len(string) and create char array to store string
	int n = s.length();
	char cursor[n + 1];

	// Copy contents into char array
	std::strcpy(cursor, s.c_str());

	// Pointer to iterate array
	char* CurPtr = cursor;
	int length = 0;

	// Determine sign of input
	if (*CurPtr == '-'){
		signum = -1;
	}

	else if (*CurPtr == '+'){
		signum = 1;
	}

	else{
		signum = 1;
	}
	
	if (*CurPtr == '-' || *CurPtr == '+'){
		CurPtr++;
	}
	
    // Ignore leading zeros
	while (*CurPtr == '0'){
		CurPtr++;
	}

    // Count num digits
	while (*CurPtr != '\0'){
		if(!std::isdigit(*CurPtr)){
			throw std::invalid_argument("BigInteger Constructor Error: non-numeric string");
		}

		length++;
		CurPtr++;
	}
	
	if (length == 0){
		throw std::invalid_argument("BigInteger Constructor Error: empty string");
	}
	
    // Find # of entries needed for list
	int exponent = (length - 1) / power;
	int NumEntries = exponent + 1;
	
	CurPtr = cursor;
	
	if (*CurPtr == '-' || *CurPtr == '+'){
		CurPtr++;
	}
	
	while (*CurPtr == '0'){
		CurPtr++;
	}
	
    // Traverses the string and inserts digits into list
	for (int i = 1; i <= NumEntries; i++){

        // Mark end of string with NULL char
		char str[9] = {'\0'};

        // First Entry
		if (i == 1){

			if (length % power != 0) {
				int numDigits = length % power;

				for (int j = 1; j <= numDigits; j++){
                    // Convert char digit to int and append to temp string
                    // ASCIIzero = ASCII value of char '0' (define as const val later)
					sprintf((str + strlen(str)), "%d", (*CurPtr - ASCIIzero));
					CurPtr++;
				}

				long result = strtol(str, NULL, 10);
				digits.insertBefore(result);
				continue;
			}
		}

        // Reset temp string after entry then append digits to temp string
		strncpy(str, "", sizeof(str));

		for (int j = 1; j <= power; j++){
			sprintf((str + strlen(str)), "%d", (*CurPtr - ASCIIzero));
			CurPtr++;
		}

        // Turn temp string to long int and insert to list
        // strtol = 'string to list'
		long result = strtol(str, NULL, 10);
		digits.insertBefore(result);
	}
}

BigInteger::BigInteger(const BigInteger& N) {
	signum = 0;
    this->signum = N.signum;
    this->digits = N.digits;
}

// Access functions --------------------------------------------------------

int BigInteger::sign() const {
    return (signum);
}

int BigInteger::compare(const BigInteger& N) const {
    List A = this->digits;
    List B = N.digits;

    if (this->sign() < N.sign())
        return -1;

    if (N.sign() < this->sign())
        return 1;

    int numReturn = 0;
    if (A.length() != B.length())
        numReturn = A.length() > B.length() ? 1 : -1;

    else {
        A.moveFront();
        B.moveFront();

        while (A.position() != A.length()) {
            long thisElement = A.peekNext();
            long nElement = B.peekNext();

            if (thisElement != nElement) {
                numReturn = thisElement > nElement ? 1 : -1;
                break;
            }
			
            A.moveNext();
            B.moveNext();
        }
    }

    return (this->sign() == -1) ? -numReturn : numReturn;
}

// Manipulation procedures -------------------------------------------------

void BigInteger::makeZero() {
    signum = 0;
    digits.clear();
}

void BigInteger::negate() {
    if (signum == -1) {
		signum = 1;
	}

	else if (signum == 1) {
		signum = -1;
	}
}

// BigInteger Arithmetic operations ----------------------------------------

void sumList(List& S, List A, List B, int sgnOfA, int sgnOfB){
    A.moveBack();
    B.moveBack();
    long carry = 0;

    // Iterate through both lists until one is exhausted
    while (A.position() != 0 && B.position() != 0) {
        long sum = (A.peekPrev() * sgnOfA) + (B.peekPrev() * sgnOfB) + carry;
        S.insertAfter(sum % base);
        carry = sum / base;
        A.movePrev();
        B.movePrev();
    }

    // Handle remaining digits in A
    while (A.position() != 0) {
        long sum = (A.peekPrev() * sgnOfA) + carry;
        S.insertAfter(sum % base);
        carry = sum / base;
        A.movePrev();
    }

    // Handle remaining digits in B
    while (B.position() != 0) {
        long sum = (B.peekPrev() * sgnOfB) + carry;
        S.insertAfter(sum % base);
        carry = sum / base;
        B.movePrev();
    }

    // If there's a carry after processing all digits, insert it
    if (carry > 0) {
        S.insertAfter(carry);
    }
}

int normalizeList(List& L){
    int sign = 1;

    // Normalize negative numbers
    if (L.front() < 0) {
        sign = -1;
        L.moveFront();
        while (L.position() != L.length()) {
            L.setAfter(L.peekNext() * -1);
            L.moveNext();
        }
    }

    // Perform carries
    long carry = 0;
    L.moveBack();
    while (L.position() != 0) {
        long current = L.peekPrev() + carry;
        carry = current / base;
        L.setBefore(current % base);
        L.movePrev();
    }

    // Handle remaining carry
    if (carry > 0) {
        L.moveFront();
        L.insertBefore(carry);
    }

    // Remove leading zeros
    L.moveFront();
    while (L.position() != L.length()) {
        if (L.peekNext() != 0) {
            break;
        }
        L.eraseAfter();
        L.moveFront();
    }

    // If the list is empty, insert a zero
    if (L.length() == 0) {
        L.insertAfter(0);
        sign = 0;
    }

    return sign;
}

// Prepends p zero digits to L, multiplying L by base^p. Used by mult().
void shiftList(List& L, int p) {
	L.moveFront();
	
	while (p-- > 0) {
		L.insertBefore(0);
	}
}

// Multiplies L (considered as a vector) by m. Used by mult().
void scalarMultList(List& L, ListElement m) {
	L.moveFront();

	while (L.position() != L.length()) {
		L.setAfter(L.peekNext() * m);
		L.moveNext();
	}
}

void negateList(List& L) {
    L.moveFront();

    while (L.position() != L.length()) {
        L.setAfter(L.peekNext() * -1);
        L.moveNext();
    }
}

// Custom helper function to check if list is empty
bool isZero(const List &digits)
{
    if (digits.length() == 0)
    {
        return true;
    }
    return false;
}

// Need to Fix: carry causes appended 0's to be removed
BigInteger BigInteger::add(const BigInteger& N) const{
	BigInteger result;
	sumList(result.digits, this->digits, N.digits, this->sign(), N.sign());
	result.signum = normalizeList(result.digits);

	if (result.digits.front() == 0 && result.digits.length() == 1) {
		result.signum = 0;
	}

	return result;
}

BigInteger BigInteger::sub(const BigInteger& N) const {
    BigInteger diff = N;
    diff.BigInteger::negate();

    return this->BigInteger::add(diff);
}

BigInteger BigInteger::mult(const BigInteger& N) const {
    BigInteger M;
    M.signum = (this->signum == N.signum) ? 1 : -1;

    // Create copies of this and N and move cursors to the back of the lists.
    List A = this->digits;
    A.moveBack();
    List B = N.digits;
    B.moveBack();

    BigInteger temp;
    int counter = 0;

    while (B.position() != 0) {
        temp.digits.clear();

        // Insert zeros to the start of temp based on counter value.
        for (int i = 0; i < counter; i++) {
            temp.digits.insertAfter(0);
        }

        A.moveBack();
        while (A.position() != 0) {
            temp.digits.insertAfter(B.peekPrev() * A.peekPrev());
            A.movePrev();
        }

        temp.signum = normalizeList(temp.digits);
        M = M.BigInteger::add(temp);
		// while (M.digits.length() % 9 != 0) {
		// 	M.digits.moveFront();
		// 	M.digits.insertBefore(0);
		// }
        //M.signum = normalizeList(M.digits);
        B.movePrev();
        counter++;
    }

    if (this->sign() == N.sign()) {
        M.signum = 1;
    }

	else if (isZero(B) || isZero(A))
    {
        M.signum = 0;
    }

    else {
        M.signum = -1;
    }

    return M;
}

// Other Functions ---------------------------------------------------------

std::string BigInteger::to_string() {
    std::string s = "";

    if (this->sign() == 0) {
        s += "0";
    }

    // Leading negative sign
    else if (this->sign() == -1) {
        s += "-" + digits.List::to_string();
    }

    // // Split number by groups of 9 starting at back
    // this->digits.movePrev();

    // // Compare segments of 9 digits and remove leading zeros from front to have result without unnecessary zeros
    // std::string seg = std::to_string((this->digits.peekNext()));

    // // If leading zero is meant to be in the segment do not remove
    // while (s[0] != '0' && s.length() != 0) {
        
    // }

    else { 
        s += digits.List::to_string();
    }

    return s;
}

// Overriden Operators -----------------------------------------------------

std::ostream& operator<<( std::ostream& stream, BigInteger N ) {
    return stream << N.BigInteger::to_string();
}

bool operator==( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == 0) {
        return true;
    }

    return false;
}

bool operator<( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == -1) {
        return true;
    }

    return false;
}

bool operator<=( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == -1 || A.compare(B) == 0) {
        return true;
    }

    return false;
}

bool operator>( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == 1) {
        return true;
    }

    return false;
}

bool operator>=( const BigInteger& A, const BigInteger& B ) {
    if (A.compare(B) == 1 || A.compare(B) == 0) {
        return true;
    }

    return false;
}

BigInteger operator+( const BigInteger& A, const BigInteger& B ) {
    return A.add(B);
}

BigInteger operator+=( BigInteger& A, const BigInteger& B ) {
    A = A.add(B);
    return A;
}

BigInteger operator-( const BigInteger& A, const BigInteger& B ) {
    return A.sub(B);
}

BigInteger operator-=( BigInteger& A, const BigInteger& B ) {
    A = A.sub(B);
    return A;
}

BigInteger operator*( const BigInteger& A, const BigInteger& B ) {
    return A.mult(B);
}

BigInteger operator*=( BigInteger& A, const BigInteger& B ) {
    A = A.mult(B);
    return A;
}