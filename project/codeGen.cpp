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
  //ensure we're at the root token
  cgAST.returnToRoot();
  segment(cgAST.rootToken);

}

//returns a segment of code and appends it to the segment that called it
  //information that is also passed (in vector size) is how many instructions
  //it contains(useful in jumps)
vector<string> CodeGen::segment(Token *a)
{
  //initialize the segment that we'll return
  vector<string> returnSegment = {};

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
        returnSegment.push_back(LDA_C); //A9
        returnSegment.push_back(BRK); //00
        returnSegment.push_back(STA); //8D

        //fill with temp memory location
        string tempVar = sdTable.addRow(a);

        returnSegment.push_back(tempVar);
        returnSegment.push_back("XX");
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
    else //token we're looking at is not the first child
    {
      //do nothing because we don't want to process the tree twice
    }

    //check for EOP
    if(a->getData() == "$")
    {
      //wrap up and do back patching
      //TODO: implement back-patching
    }

    //recursion
    for(vector<Token*>::size_type i = 0; i < a->children.size(); i++)
    {
      //store recursion results in string vector
      vector<string> recursionSegment = segment(a->children[i]);
      //push each string to the back of the vector, in order
      for(vector<string>::size_type j = 0; j < recursionSegment.size(); j++)
      {
        returnSegment.push_back(recursionSegment[j]);
      }
    }

    //return the result of the segment
    return returnSegment;



  }
  else //token is a branch node
  {
    //if we reach an if statement, then handle a jump and call segment on it's children
    if(a->getType() == "IfStatement")
    {
      //process conditional

      //run segment on ifBlock children
      vector<string> ifBlock = segment(a->children[1]);

      //Jump stuff, I don't think I really need it with my method
      unsigned int jumpDistance = static_cast<unsigned int>(ifBlock.size());
      //create new jump
      jTable.addRow(jumpDistance);

      //push back segment to return segment
      for(vector<string>::size_type i = 0; i < ifBlock.size(); i++)
      {
        returnSegment.push_back(ifBlock[i]);
      }
    }

    return returnSegment;

  }
}

//compare stack head and heap head and make sure there is not an overflow
void CodeGen::checkForOverFlow()
{

}

//replace temporary variable and jump names with actual memory locations
void CodeGen::backPatching()
{

}

