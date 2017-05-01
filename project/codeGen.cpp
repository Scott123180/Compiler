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
    printBoolHell = true;
    vector<string> errorData = {};
    Error booleanHell = Error(false, Error::codeGen,0,0,errorData, "Nested bool detected. WHY?!");
  }
  else
  {
    //process the code
    process();

    //check for overflow after execution
    if(overFlow)
    {
      vector<string> errorData = {};
      Error booleanHell = Error(true, Error::codeGen,0,0,errorData,
      "Overflow error: program output code longer than 256 bytes.");
    }
  }


}

//start processing tokens
void CodeGen::process()
{
  //ensure we're at the root token
  cgAST.returnToRoot();
  loop(cgAST.rootToken);

}

void CodeGen::loop(Token *a)
{
  //check for leaf node
  if(!a->getData().empty()) //(not a branch node)
  {
    //check parent
    string parent = a->parent->getType();

    //check for 
  }

  //recursion
  for(vector<Token*>::size_type i = 0; i < a->children.size(); i++)
  {
    loop(a->children[i]);
  }
}