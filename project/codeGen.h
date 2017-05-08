#ifndef COMPILER_CODEGEN_H
#define COMPILER_CODEGEN_H

#include <sstream>
#include <iomanip>

#include "cst.h"
#include "symbolTable.h"
#include "jumps.h"
#include "staticData.h"
#include "error.h"
#include "genSymbolTable.h"


class CodeGen
{
  public:
    CodeGen(CST* ast, GenSymbolTable* st);

    ~CodeGen();

    string output[256];

    //for use in printing bool hell warning
    bool printBoolHell = false;

    vector<string> segment(Token* a);

    string getVariableType(Token* a);

    vector<string> assignExpressionSegment(Token* expr, string tempVarName);

    vector<string> assignIntExpressionSegment(Token* a, string tempVarName);
    void assignIntExpressionLoop(Token* a);
    vector<string> assignIntExpressionTerminals;

    vector<string> assignBooleanExpressionSegment(Token* a, string tempVarName);

    vector<string> assignStringExpressionSegment(Token* a, string tempVarName);

    vector<string> printExpressionSegment(Token* a);

    vector<string> printIntExpressionSegment(Token* a);

    vector<string> printBooleanExpressionSegment(Token* a);

    vector<string> printStringExpressionSegment(Token* a);


  private:

    CST* cgAST;

    SymbolTable* cgSymbolTable;

    GenSymbolTable* cgGenSymbolTable;

    StaticData sdTable = StaticData();

    Jumps jTable = Jumps();

    vector<string> code = {};

    unsigned long int codeSize;

    vector<string> stack = {};

    unsigned long stackSize;

    //keep track of program exceeding 256 bytes
    bool overFlow = false;

    //heap will always at least be size of one because it's easier to implement
      //heaphead keeps index of the last character written to heap
    int heapHead = 256;

    void backPatching();

    void allocateMemoryOnStack();

    void replaceTemporaryMemoryAddresses();

    void replaceTemporaryJumpAddresses();

    vector<string> stringToHexChars(string a);

    string intToHex(int a);

    int hexToInt(string hexValue);
    
    void checkForOverFlow();

    void printCode();

    void process();


    ///op code section

    //load the accumulator with a constant
    const string LDA_C = "A9";
    //load the accumulator from memory
    const string LDA_M = "AD";
    //store the accumulator in memory
    const string STA = "8D";
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
    
    const string XX = "XX"; //used in temporary addresses, replaced later
};

#endif //COMPILER_CODEGEN_H
