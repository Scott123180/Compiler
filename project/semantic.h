#ifndef COMPILER_SEMANTIC_H
#define COMPILER_SEMANTIC_H

#include <vector>
#include <string>
#include <iostream>

#include "token.h"
#include "cst.h"
#include "symbolTable.h"

class Semantic
{
  public:
    Semantic(vector<Token> stream, bool v, unsigned int start);  //v is for verbose
    unsigned int i;

    vector<Token> tokens;

    //I know it's terrible naming, let's get it working before we fix this
    //TODO: change CST naming. Lol let's see if I actually get around to doing this
    CST newAST;

    void kick(); //kick current token parent token

    void kickST(); //kick current symbolTable to parent symboltable

    vector<string> symbolTableOuput;

    //for calculation in dfio
    SymbolTable* calcDepth = nullptr;

    bool verbose;

    //output of symbol table with dashes
    void calcSymbolTableOutput(SymbolTable* a, bool verbose);

    //current symbolTable
    SymbolTable* curSymbolTable;

    //counter to make every scope have a distinct name
    unsigned int uniqueScope = 0;

    //keeps track of type of last token in when declaring variables
    string typeBuffer;

    //if in expr, what type of expr is it
      //so that we can check we're not adding ints to strings or bool
      //because expressions can turn into other expressions
    bool inIntExpr = false;
    bool inBoolExpr = false;
    bool inStringExpr = false;

    void resetInExpr(){inIntExpr = false; inBoolExpr = false; inStringExpr = false;}

  private:

    //recursive decent functions
    bool term(string tt); //terminal


    bool Program1();
    bool Program();

    bool Block1();
    bool Block();

    bool StatementList1();
    bool StatementList2(); //epsilon
    bool StatementList();

    bool Statement1();
    bool Statement2();
    bool Statement3();
    bool Statement4();
    bool Statement5();
    bool Statement6();
    bool Statement();

    bool PrintStatement1();
    bool PrintStatement();

    bool AssignmentStatement1();
    bool AssignmentStatement();

    bool VarDecl1();
    bool VarDecl();

    bool WhileStatement1();
    bool WhileStatement();

    bool IfStatement1();
    bool IfStatement();

    bool Expr1();
    bool Expr2();
    bool Expr3();
    bool Expr4();
    bool Expr();

    bool IntExpr1();
    bool IntExpr2();
    bool IntExpr();

    bool StringExpr1();
    bool StringExpr();

    bool BooleanExpr1();
    bool BooleanExpr2();
    bool BooleanExpr();

    bool Id1();
    bool Id();

    bool CharList1();
    bool CharList2(); //epsilon
    bool CharList();

    bool type1();
    bool type();

    bool Char1();
    bool Char();

    bool space1();
    bool space();

    bool digit1();
    bool digit();

    bool boolop1();
    bool boolop();

    bool boolval1();
    bool boolval();

    bool intop1();
    bool intop();
};

#endif