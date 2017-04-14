#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <vector>

#include "stEntry.h"
#include "error.h"

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

    void assignVarTable(StEntry e);

    StEntry& lookupEntry(char a, SymbolTable* s);

  private:
};

#endif