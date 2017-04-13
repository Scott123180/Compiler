#include <iostream>
#include <vector>

#include "symbolTable.h"
#include "semantic.h"
#include "stEntry.h"
#include "error.h"

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
    else //variable not yet declared
    {
      //create StEntry object and push back to symbolTable
      if(e.type == "int")
      {
        s->rows.push_back(StEntry(e.name, e.type, e.lineNum, s->scope, false, e.getDigit()));
      }
      else if(e.type == "string")
      {
        s->rows.push_back(StEntry(e.name, e.type, e.lineNum, s->scope, false, e.getcharList()));
      }
      else //boolean
      {
        s->rows.push_back(StEntry(e.name, e.type, e.lineNum, s->scope, false, e.getBoolean()));
      }
    }
  }
}

void SymbolTable::assignVarTable(StEntry e)
{
  StEntry& assignee = this->lookupEntry(e.name, this);
  //check if the assignee was found in scopes
  if(assignee.name == '\0')
  {
    vector<string> errorData = {""};
    //throw error UNINITIALIZED VARIABLE
    Error assignError(true, Error::errorStage::semantic, e.lineNum, 0, errorData, "Variable attempting to assign to has not been declared");
  }
  else  ///FOUND THE VARIABLE
  {
    //check type
    if(e.type != assignee.type) //types don't match
    {
      vector<string> errorData = {""};
      //throw error TYPE MISMATCH
      Error assignError(true, Error::errorStage::semantic, e.lineNum, 0, errorData, "Type mismatch error.");
    }
    else ///VALID TYPE
    {
      if(assignee.type == "int")
      {
        assignee.setDigit(e.getDigit()); //assign digit of the template obj
      }
      else if(assignee.type == "string")
      {
        assignee.setCharList(e.getcharList()); //assign charList of the template obj
      }
      else //boolean value
      {
        assignee.setBoolean(e.getBoolean()); //assign boolean of the template obj
      }
    }
  }
}

//returns an StEntry if found in any scope, otherwise returns a dud StEntry
StEntry& SymbolTable::lookupEntry(char a, SymbolTable* s)
{
  //loop through vector to find
  for(vector<StEntry>::size_type i = 0; i < s->rows.size(); i++)
  {
    //compare names
    if(s->rows[i].name == a)
    {
      //found it
      return s->rows[i]; //return StEntry
    }
  }
  //not found in this scope
  if(s->parent) //check if there is a parent scope (check for no nullptr)
  {
    //recursion
    return s->parent->lookupEntry(a, s->parent);
  }
  else
  {
    //make a dud and then return that value
    StEntry notFound('\0', "", 0, 0, false);
  }
}