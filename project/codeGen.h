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

    string output[256];

    //for use in printing bool hell warning
    bool printBoolHell = false;

    int curIndex = 0;

    void loop(Token* a);

  private:

    CST cgAST;

    SymbolTable* cgSymbolTable;

    //keep track of program exceeding 256 bytes
    bool overFlow = false;

    void process();

    ///op code section

    //load the accumulator with a constant
    const string LDA_C = "A9";
    //load the accumulator from memory
    const string LDA_M = "AD";
    //store the accumulator in memory
    const string STA = "AD";
    //Add with carry
    const string ADC = "6D";
    //load the X register with a constant
    const string LDX_C = "A2";
    //load the X register from memory
    const string LDX_M = "AE";
    //load the Y register with a constant
    const string LDY_C = "A0";
    //load the Y register from memory
    const string LDY_M = "AC";
    //no operation
    const string NOP = "EA";
    //break (which is really a system call)
    const string BRK = "00";
    //compare a byte in memory to the X reg - Sets the Z(zero) flag if equal
    const string CPX = "EC";
    //branch n bytes if Z flag = 0
    const string BNE = "D0";
    //increment the value of a byte
    const string INC = "EE";
    //System calls
    const string SYS = "FF";
      //01 in x reg = print the integer stored in the y register
      const string P_INT = "01";
      //02 in x reg = print the 00-terminated string stored at the address in the y register
      const string P_STR = "02";
};

#endif //COMPILER_CODEGEN_H
