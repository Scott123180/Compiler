#include <iostream>
#include <string>

#include "lexer.h"
#include "error.h"
#include "output.h"

using namespace std;

int main(int argc, char** argv)
{
  if(argc != 2)//make sure input file is specified
  {
    Error newError(true, newError.fileInput, 0, 0, "", "Must have the name of a text file"
      " for input with no spaces as only argument.");
  }

  string fileName = argv[1];

  Lexer newLex(fileName);

  Output newOutput;
  newOutput.printOutput(newLex.stream);

  //TODO: parse and CST functions

}