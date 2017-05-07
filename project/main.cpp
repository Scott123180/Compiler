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
#include "genSymbolTable.h"
#include "codeGen.h"

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
  Semantic newSemantic(newLex.stream, false, 0);

  cout << "-->Before CodeGen" << endl;
  //generate code from AST
  // (scope is stored in tokens of var, boolean hell error detection stored in ST)
  CodeGen newCodeGen(&newSemantic.newAST, newSemantic.newGen);

  cout << "->After CodeGen" << endl;
  cout << "->Before output" << endl;


  Output newOutput(newLex, newParser, newSemantic, newCodeGen);
  newOutput.printOutput();

  cout << "->After output"<< endl;

  /* CleanUp Objects and Pointers */


}