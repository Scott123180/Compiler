#include <iostream>
#include <string>

#include "lexer.h"
#include "error.h"

int main(int argc, char** argv)
{

    if(argc != 2)//make sure input file is specified
    {
      Error::genError(fileInput, 0, 0, "", "Must have the name of a text file"
        " for input with no spaces as only argument.");
    }

    string fileName = argv[1];

    Lexer::Lex(fileName);

    //TODO: parse and CST functions

}