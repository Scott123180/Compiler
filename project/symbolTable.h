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

    SymbolTable* parent;
    vector<SymbolTable*> children;

    unsigned int scope;

    vector<StEntry> rows;

    void declVarTable();

    void assignVarTable();

    StEntry lookupEntry(char a);
    
  private:
};

#endif