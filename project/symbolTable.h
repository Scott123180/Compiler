#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <vector>

#include "stEntry.h"
#include "error.h"
#include "token.h"

using namespace std;

class SymbolTable
{
  public:
    SymbolTable(SymbolTable* parent, unsigned int uniqueS);

    SymbolTable* parent; //initialize with no parent
    vector<SymbolTable*> children;

    unsigned int calcTableDepth(SymbolTable* a);

    unsigned int scope;

    vector<StEntry> rows;

    void declVarTable(StEntry e, SymbolTable* s);

    string assignVarTable(StEntry e);

    StEntry* lookupEntry(char a, SymbolTable* s);

    //error checking and warning functions
    void usedNotDeclared(StEntry* a, Token* b); //error

    void declaredNotUsed(); //warning

    //pass root symbol table and dfio traversal checking for type mismatch
      //(more than one data type assigned to StEntry)
    void typeMismatch(); //error

  private:
};

#endif