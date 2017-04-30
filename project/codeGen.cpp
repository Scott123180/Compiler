#include "codeGen.h"
#include "error.h"

using namespace std;

CodeGen::CodeGen(CST ast, SymbolTable* st)
  :cgAST(ast), cgSymbolTable(st)
{
  //initialize output array to 00's
  for(int i = 0; i < 256; i++)
  {
    output[i] = "00";
  }

  //check to see if boolean hell flag is set in the symbol table
  if(st->booleanHell)
  {
    vector<string> errorData = {};
    Error booleanHell = Error(true, Error::codeGen,0,0,errorData, "Boolean Hell detected. WHY?!");
  }

  //process the code
  process();

}


void CodeGen::process()
{

}