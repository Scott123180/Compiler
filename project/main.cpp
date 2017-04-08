#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"
#include "error.h"
#include "output.h"
#include "parser.h"
#include "cst.h"
#include "symbolTable.h"
#include "semantic.h"

using namespace std;

int main(int argc, char** argv)
{
  if(argc != 2)//make sure input file is specified
  {
    vector<string> empty = {""};
    Error newError(true, newError.fileInput, 0, 0, empty, "Must have the name of a text file"
      " for input with no spaces as only argument.");
  }

  string fileName = argv[1];
  
  Lexer newLex(fileName);
  
  //set boolean to true for verbose, false for not verbose
  Parser newParser(newLex.stream, false,0);

  //set boolean to true for verbose, false for not verbose
  Semantic newSemantic(newLex.stream, true, 0);


  Output newOutput;
  newOutput.printOutput(newLex.stream, newParser.newCST.tree, newSemantic.newAST.tree);

}