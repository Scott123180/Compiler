#ifndef COMPILER_CODEGEN_H
#define COMPILER_CODEGEN_H

#include "cst.h"
#include "symbolTable.h"
#include "jumps.h"
#include "staticData.h"

class CodeGen
{
  public:
    CodeGen(CST ast, SymbolTable* st);

  private:

};

#endif //COMPILER_CODEGEN_H
