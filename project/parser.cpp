

#include "parser.h"
#include "token.h"

Parser::Parser(vector<Token> stream)
{
  int i = 0;
  /*
   * next is the current token we're working with, we increment it every time just
   * in case we find a match, but then set it back if we have to check again if a term
   * matches
   *
   * term checks if something matches a term
   */
  //initialize next to point to the first token
  //invoke E()
  static Token next = stream[i].returnToken();

  Program();



}
  bool Program1() { return Block() && term("$"); }

  bool Program()
  {
    //TODO: next and run expressions
  }

  bool Block1() { return term("{") && StatementList() && term("}"); }
  bool Block2() { return true; }//epsilon

  bool Block()
  {
    //TODO: next and run expressions
  }

  bool Statement1() { return PrintStatement(); }
  bool Statement2() { return AssignmentStatment(); }
  bool Statement3() { return VarDecl(); }
  bool Statement4() { return WhileStatement(); }
  bool Statement5() { return IfStatement(); }
  bool Statement6() { return Block(); }

  bool Statement()
  {
    //TODO: next and run expressions
  }

  bool PrintStatement1(){ return term("print") && term("(") && Expr() && term(")"); }

  bool PrintStatement()
  {
    //TODO: next and run expressions
  }

  bool AssignmentStatement1() { return Id() && term("=") && Expr(); }

  bool AssignmentStatement()
  {
    //TODO: next and run expressions
  }

  bool VarDecl1() { return type() && Id(); }
  bool VarDecl()
  {
    //TODO: next and run expressions
  }