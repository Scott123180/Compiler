#ifndef COMPILER_GEN_SYMBOL_TABLE
#define COMPILER_GEN_SYMBOL_TABLE

#include <vector>
#include <string>

#include "cst.h"
#include "symbolTable.h"
#include "token.h"
#include "stEntry.h"

class GenSymbolTable
{
  public:
    //take in a semantic object
    GenSymbolTable(Token* r, bool v);

    ~GenSymbolTable();

    Token* rootToken;

    bool verbose;

    unsigned int uniqueScope = 0;

    //root symbol Table
    SymbolTable* rootSymbolTable;

    //current symbolTable
    SymbolTable* curSymbolTable;

    //current token
    Token* curToken;

    vector<string> symbolTableOutput;

    //delete all tables
    void deleteAllSTChildren(SymbolTable* a);

    //search through tree dfio
    void produceST(Token* a);

    bool firstBlock = true;

    //Block
    bool blockST();
    //VarDecl
    bool varDeclST();
    //Assign
    bool assignST();
    //If
    bool ifST();
    //Print
    bool printST();

    bool whileST();

    //return types for expressions
    //Expr
    string exprST(Token* a);
    //BoolExpr
    string boolExprST(Token* a);
    //IntExpr
    string intExprST(Token* a);
    //StringExpr
    string stringExprST();
    //output of symbol table with dashes
    void calcSymbolTableOutput(SymbolTable* a, bool verbose);

    void kickTok(); //kick token to parent

    void kickST(); //kick symbol table to parent

    /*
     * helper functions for CodeGen
    */

    string returnType(Token* a);

    SymbolTable* match = nullptr;

    void matchScope(SymbolTable* a, int scopeLookup);

  private:

    vector<Token> tokens;

};

#endif