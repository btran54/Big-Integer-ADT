/***
* Brian Tran
* btran54
* 2024 Winter CSE101 PA6
* Arithmetic.cpp
* Arithmetic CPP file containing the main execution of BigInteger.cpp functions.
***/ 

#include <string>
#include <iostream>
#include <fstream>

#include "BigInteger.h"
#include "List.h"

using namespace std;

int main(int argc, char * argv[]) {
    if (argc != 3) {
        cerr << "Invalid Arguments.\nUsage: " << argv[0] << "<input file> <output file>" << endl;
        return EXIT_FAILURE;
    }

    ifstream in;
    ofstream out;
    in.open(argv[1]);
    out.open(argv[2]);

    if (!in.is_open()) {
        cerr << "Unable to open file " << argv[1] << " for reading.\n" << endl;
        return EXIT_FAILURE;
    }

    if (!out.is_open()) {
        cerr << "Unable to open file " << argv[2] << " fo writing.\n" << endl;
        return EXIT_FAILURE;
    }

    string lineRead;
    getline(in, lineRead);
    BigInteger A = BigInteger(lineRead);
    getline(in, lineRead);
    getline(in, lineRead);
    BigInteger B = BigInteger(lineRead);

    out << A << endl;
    out << endl;

    out << B << endl;
    out << endl;

    out << A + B << endl;
    out << endl;

    out << A - B << endl;
    out << endl;

    out << A - A << endl;
    out << endl;

    out << (3 * A) - (2 * B) << endl;
    out << endl;

    out << A * B << endl;
    out << endl;

    out << A * A << endl;
    out << endl;

    out << B * B << endl;
    out << endl;

    out << (9 * (A * A * A * A)) + (16 * (B * B * B * B * B)) << endl;
    out << endl;


    // out << ((A * A * A)) << endl;
    // out << ((A * A * A * A)) << endl;
    // out << ((B * B )) << endl;
    // out << ((B * B * B)) << endl;
    // out << ((B * B * B * B)) << endl;
    // out << ((B * B * B * B * B)) << endl;

    in.close();
    out.close();

    return EXIT_SUCCESS;
}