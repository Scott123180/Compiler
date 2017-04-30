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

    ///op code section

    //load the accumulator with a constant
    string LDA_C = "A9";
    //load the accumulator from memory
    string LDA_M = "AD";
    //store the accumulator in memory
    string STA = "AD";
    //Add with carry
    string ADC = "6D";
    //load the X register with a constant

    //load the X register from memory

    //load the Y register with a constant

    //load the Y register from memory

    //no operation

    //break (which is really a system call)

    //compare a byte in memory to the X reg - Sets the Z(zero) flag if equal

    //branch n bytes if Z flag = 0

    //increment the value of a byte

    //System calls

      //01 in x reg = print the integer stored in the y register

      //03 in x reg = print the 00-terminated string stored at the address in the y register
};

#endif //COMPILER_CODEGEN_H
