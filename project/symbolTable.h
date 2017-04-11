#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <vector>

#include "stEntry.h"
#include "error.h"

using namespace std;

class SymbolTable
{
  public:
    SymbolTable(unsigned int scope);

    SymbolTable* parent = nullptr; //initialize with no parent
    vector<SymbolTable*> children;

    unsigned int scope;

    vector<StEntry> rows;

    void declVarTable();

    void assignVarTable(StEntry e);

    StEntry lookupEntry(char a, SymbolTable* s);

  private:
};

#endif