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
    GenSymbolTable(Semantic s, bool v);

    bool verbose;

    unsigned int uniqueScope = 0;

    vector<string> symbolTableOuput;

    //root symbol Table
    SymbolTable* rootSymbolTable;

    //current symbolTable
    SymbolTable* curSymbolTable;

    //root token
    Token* rootToken;

    //current token
    Token* curToken;

    vector<string> symbolTableOutput;

    //search through tree dfio
    void produceST(Token* a);

    //process token
    void processToken(Token* a);
    //VarDecl

    //Assign

    //Block

    //If

    //Print

    //Expr

    //BoolExpr

    //IntExpr

    //StringExpr

    //output of symbol table with dashes
    void calcSymbolTableOutput(SymbolTable* a, bool verbose);

    void kickTok(); //kick token to parent

    void kickST(); //kick symbol table to parent

    StEntry varLookup(string a, bool utilizeIt)
    {

    }

    void declVar(string a, string type, int lineNum, int pos);

    void assignVar(Token* a);

  private:

    vector<Token> tokens;

};

#endif