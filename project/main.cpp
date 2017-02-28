#include <iostream>
#include <fstream>

#include "main.h"

int main(int argc, char** argv)
{
    if(argc != 2)//make sure input file is specified
    {
        cout << "Must have the name of a text file for input with no spaces as only argument" << endl;
        return 1;
    }
    string fileName = argv[1];

    Lex(fileName);

    //TODO: parse and CST functions

}