#ifndef COMPILER_GEN_SYMBOL_TABLE
#define COMPILER_GEN_SYMBOL_TABLE

#include <vector>
#include <string>

#include "cst.h"
#include "semantic.h"
#include "symbolTable.h"
#include "token.h"
#include "stEntry.h"

class GenSymbolTable
{
  public:
    //take in a semantic object
    GenSymbolTable(Token* r, bool v);

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
    void deleteAllST(SymbolTable* a);

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

  private:

    vector<Token> tokens;

};

#endif