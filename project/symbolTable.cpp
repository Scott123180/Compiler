#include <iostream>
#include <vector>

#include "symbolTable.h"
#include "stEntry.h"
#include "error.h"

SymbolTable::SymbolTable(unsigned int scope)
{
  return;
}

void SymbolTable::declVarTable()
{
  //check that variable hasn't been declared in scope

  //create StEntry object

  //add StEntry object to rows in current symbolTable
  
  return;
}

void SymbolTable::assignVarTable(StEntry e)
{
  StEntry* assignee = this->lookupEntry(e.name, this);
  //check if the assignee was found in scopes
  if(assignee.name == '')
  {
    vector<string> errorData = {""};
    //throw error UNINITIALIZED VARIABLE
    Error assignError(true, Error::errorStage::semantic, e.lineNum, 0, errorData, "Variable attempting to assign to has not been declared");
  }
  else  ///FOUND THE VARIABLE
  {
    //check type
    if(e.type != assignee->type) //types don't match
    {
      vector<string> errorData = {""};
      //throw error TYPE MISMATCH
      Error assignError(true, Error::errorStage::semantic, e.lineNum, 0, errorData, "Type mismatch error.");
    }
    else ///VALID TYPE
    {
      if(assignee->type == "int")
      {
        assignee->setDigit(e.getDigit()); //assign digit of the template obj
      }
      else if(assignee->type == "string")
      {
        assignee->setCharList(e.getcharList()); //assign charList of the template obj
      }
      else //boolean value
      {
        assignee->setBoolean(e.getBoolean()); //assign boolean of the template obj
      }
    }
  }
}

//returns an StEntry if found in any scope, otherwise returns a dud StEntry
StEntry &SymbolTable::lookupEntry(char a, SymbolTable* s)
{
  //loop through vector to find
  for(vector<StEntry>::size_type i = 0; i < s.rows.size(); i++)
  {
    //compare names
    if(rows[i].name == a)
    {
      //found it
      return &rows[i]; //return StEntry
    }
  }
  //not found in this scope
  if(s.parent) //check if there is a parent scope (check for no nullptr)
  {
    //recursion
    return &s->parent->lookupEntry(a, s->parent);
  }
  else
  {
    //make a dud and then return that value
    StEntry notFound('', "", 0, 0, false);
  }
}