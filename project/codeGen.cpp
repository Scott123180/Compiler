#include "codeGen.h"
#include "error.h"

using namespace std;

CodeGen::CodeGen(CST ast, SymbolTable* st)
  : cgAST(ast), cgSymbolTable(st)
{
  cout << "CodeGen constructor called" << endl;
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

CodeGen::~CodeGen()
{
  cout << "CodeGen Destructor Called" << endl;
}

//start processing tokens
void CodeGen::process()
{
  cout << "-------Got to process " << endl;
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
    string parentType = a->parent->getType();

    //if the token we're looking at is the first child
    if(a->parent->children[0] == a)
    {
      if(parentType == "VarDecl")
      {
        output[curIndex++] = LDA_C; //A9
        output[curIndex++] = BRK; //00
        output[curIndex++] = STA; //8D

        //fill with temp memory location
        string tempVar = sdTable.addRow(a);

        output[curIndex++] = tempVar;
        output[curIndex++] = "XX";
      }
      else if(parentType == "AssignStatement")
      {

      }
      else if(parentType == "==" || parentType == "!=")
      {

      }
      else if(parentType == "PrintStatement")
      {

      }
      else //unreachable unless I forgot something
      {
        cout << "Please edit the code to include this type" << endl;
        cout << parentType << endl;
      }
    }
    else
    {
      //do nothing
    }
  }

  //recursion
  for(vector<Token*>::size_type i = 0; i < a->children.size(); i++)
  {
    loop(a->children[i]);
  }
}