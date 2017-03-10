#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include <vector>

#include "token.h"

class Parser
{

public:
  //parser object
  Parser(vector<Token> stream);


private:
  static vector<Token> tokens;
  static int i = 0; //points to which token we're on
  
  static vector<string> expecting;

  //recursive decent functions
  bool term(string tt); //terminal


  bool Program1();
  bool Program();

  bool Block1();
  bool Block2();//epsilon
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
  bool CharList2();
  bool CharList3();
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

#endif //COMPILER_PARSER_H