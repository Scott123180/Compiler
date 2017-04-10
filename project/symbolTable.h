#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H

#include <vector>

#include "stEntry.h"
#include "error.h"

using namespace std;

class SymbolTable
{
  public:
    SymbolTable();

    SymbolTable* parent;
    vector<SymbolTable*> children;

    unsigned int depth;

    vector<StEntry> rows;

    void declVarTable();

    void assignVarTable();
  private:
};

#endif