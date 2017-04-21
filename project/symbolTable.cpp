#include <iostream>
#include <vector>

#include "symbolTable.h"
#include "semantic.h"
#include "stEntry.h"
#include "error.h"
#include "token.h"

SymbolTable::SymbolTable(SymbolTable* p, unsigned int uniqueS)
  : parent(p), scope(uniqueS)
{

}

unsigned int SymbolTable::calcTableDepth(SymbolTable* a)
{
  unsigned int depth = 0;
  SymbolTable* thisTable = a;
  //scale up tree
  while(thisTable->parent) //check for root (nullptr parent)
  {
    ++depth;
    thisTable = thisTable->parent;
  }
  return depth;
}

void SymbolTable::declVarTable(StEntry e, SymbolTable* s)
{
  //check that variable hasn't been declared in scope
  for(vector<StEntry>::size_type i = 0; i < s->rows.size(); i++)
  {
    if(s->rows[i].name == e.name) //compare name of current scope rows with template entry
    {
      //ERROR, redeclared variable
        //convert char name to string for error
      string nameOfVar = "";
      nameOfVar.push_back(e.name);
      vector<string> errorData = {nameOfVar};
      Error declVarError(true, Error::errorStage::semantic, e.lineNum, 0, errorData, "Variable Redeclaration");
    }
  }
 //variable not yet declared
  //create StEntry object and push back to symbolTable
  if(e.type == "int")
  {
    StEntry push = StEntry(e.name, e.type, e.lineNum, e.position, s->scope, true, e.getDigit());
    s->rows.push_back(push);
  }
  else if(e.type == "string")
  {
    StEntry push = StEntry(e.name, e.type, e.lineNum, e.position, s->scope, true, e.getcharList());
    s->rows.push_back(push);
  }
  else //boolean
  {
    StEntry push = StEntry(e.name, e.type, e.lineNum, e.position, s->scope, true, e.getBoolean());
    s->rows.push_back(push);

  }
}

string SymbolTable::assignVarTable(StEntry e)
{
  StEntry* assignee = this->lookupEntry(e.name, this, e.lineNum, e.position, true);

  if(assignee->type == "int")
  {
    assignee->setDigit(e.getDigit()); //assign digit of the template obj
    assignee->utilized = true;
    return "int";
  }
  else if(assignee->type == "string")
  {
    assignee->setCharList(e.getcharList()); //assign charList of the template obj
    assignee->utilized = true;
    return "string";
  }
  else //boolean value
  {
    assignee->setBoolean(e.getBoolean()); //assign boolean of the template obj
    assignee->utilized = true;
    return "boolean";
  }
  //default return value
  return "\0";
}

//returns an StEntry if found in any scope, otherwise returns a dud StEntry
StEntry* SymbolTable::lookupEntry(char a, SymbolTable* s, int tokLineNum, int tokLinePos, bool errorOnFail)
{
  //loop through vector to find
  for(vector<StEntry>::size_type i = 0; i < s->rows.size(); i++)
  {
    //compare names
    if(s->rows[i].name == a)
    {
      //found it- lol pretty sure this is how memory leaks are made
      StEntry* foundStEntry = &s->rows[i];

      return foundStEntry; //return StEntry
    }
  }
  //not found in this scope
  if(s->parent) //check if there is a parent scope (check for no nullptr)
  {
    //recursion
    return s->parent->lookupEntry(a, s->parent, tokLineNum, tokLinePos, errorOnFail);
  }
  else //fail
  {
    if(errorOnFail)
    {
      string charToString;
      charToString.push_back(a);
      vector<string> errorData = {charToString};
      Error notFound = Error(true, Error::semantic, tokLineNum, tokLinePos, errorData,
                             "Undeclared Identifier: ");
    }
    else //return nullptr
    {
      return nullptr;
    }
  }
}

//takes an StEntry and throws an error if it's empty
void SymbolTable::usedNotDeclared(StEntry *a, Token* b)
{
  //null name
  if(a->name == '\0')
  {
    int lineNum = b->getLine();
    int positionNum = b->getPos();
    vector<string> errorData = {b->getData()};
    Error undeclaredId = Error(true, Error::errorStage::semantic, lineNum, positionNum,
                               errorData, "Undeclared Identifier");
  }
}

void SymbolTable::usedNotInitialized(StEntry *a, Token* b)
{
  //check if at least one has been set
  if(a->hasIntBeenSet || a->hasBoolBeenSet || a->hasStringBeenSet)
  {
    //good
  }
  else
  {
    int lineNum = b->getLine();
    int positionNum = b->getPos();
    vector<string> errorData = {b->getData()};
    Error undeclaredId = Error(true, Error::errorStage::semantic, lineNum, positionNum,
                               errorData, "Uninitialized Identifier");
  }
}
//create warnings for variables that aren't used in table
void SymbolTable::declaredNotUsed()
{
  SymbolTable b = *this;
  for(vector<StEntry>::size_type j = 0; j < b.rows.size(); j++)
  {
    //unused variable
    if(! b.rows[j].utilized)
    {
      int lineNum = rows[j].lineNum;
      string charToString;
      charToString.push_back(b.rows[j].name);
      vector<string> errorData = {charToString};
      Error unusedVar = Error(false, Error::errorStage::semantic, lineNum, 0, errorData
                              , "Warning: variable initialized but never used");
    }
  }
}

//call me in calcSymbolTableOutput. check for type mismatch
void SymbolTable::typeMismatch()
{
  SymbolTable a = *this;
  for(vector<StEntry>::size_type j = 0; j < a.rows.size(); j++)
  {
    StEntry curEntry = a.rows[j];
    vector<string> errorData; //keep track of declared fields
    if(curEntry.hasIntBeenSet) errorData.push_back("int");
    if(curEntry.hasStringBeenSet) errorData.push_back("string");
    if(curEntry.hasBoolBeenSet) errorData.push_back("bool");
    //check if more than two set
    if(errorData.size() > 1) //mor than one type set
    {
      Error typeMismatch = Error(true, Error::semantic, curEntry.lineNum, 0, errorData
                                , "Variable can only have one type. Type mismatch: ");
    }
  }
}

//