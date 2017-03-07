

#include "parser.h"
#include "token.h"

Parser::Parser(vector<Token> stream)
  : tokens(stream)
{

  /*
   * next is the current token we're working with, we increment it every time just
   * in case we find a match, but then set it back if we have to check again if a term
   * matches
   *
   * term checks if something matches a term
   */
  //initialize next to point to the first token
  //invoke E()

  Program();



}
/*

 bool term(TOKEN tok) {return *next++ == tok; }
   returns true if the token we passed in matches the input or no it doesn't
 */

  bool term(string type) //terminal
  {
    return Token::tokens[(i++)].getType() == type;
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

  bool WhileStatement1() { return term("while") && BooleanExpr() && Block(); }

  bool WhileStatement()
  {
    //TODO: next and run expressions
  }

  bool IfStatement1() { return term("if") && BooleanExpr() && Block(); }
  bool IfStatement()
  {
    //TODO: next and run expressions
  }

  bool Expr1() { return IntExpr(); }
  bool Expr2() { return StringExpr(); }
  bool Expr3() { return BooleanExpr(); }
  bool Expr4() { return Id(); }

  bool Expr()
  {
    //TODO: next and run expressions
  }

  bool IntExpr1() { return digit() && intop && Expr(); }
  bool IntExpr2() { return digit(); }

  bool IntExpr()
  {
    //TODO: next and run expressions
  }

  bool StringExpr1() { return term("\"") && CharList() && term("\""); }
  bool StringExpr()
  {
    //TODO: next and run expressions
  }

  bool BooleanExpr1() { return term("(") && Expr() && boolop() && Expr(); }
  bool BooleanExpr2() { return boolval(); }
  bool BooleanExpr()
  {
    //TODO: next and run expressions
  }

  bool Id1() { return Char(); }
  bool Id()
  {
    //TODO: next and run expressions
  }

  bool CharList1() { return Char() && CharList(); }
  bool CharList2() { return space() && CharList(); }
  bool CharList3() { return true; }
  bool CharList()
  {
    //TODO: next and run expressions
  }

  bool type1() { term("int"); }
  bool type2() { term("string"); }
  bool type3() { term("boolean"); }
  bool type()
  {
    //TODO: next and run expressions
  }

  bool Char1()
  {
    char alphabet[] = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                        'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                        's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    for(int i = 0; i < 26; i++)
    {
      //if(alphabet[i] ==  /* TODO: match with current token */);
      //  {term(alphabet[i]; return true;}
    }
  }
  bool Char()
  {
    //TODO: next and run expressions
  }

  bool space1() { return term(" "); }
  bool space()
  {
    //TODO: next and run expressions
  }

  bool digit1()
  {
    int nums[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    for(int i = 0; i < 10; i++)
    {
      //if(nums[i] == /* TODO: match with current token */);
      //  {term(nums[i]; return true;
    }
  }
  bool digit()
  {
    //TODO: next and run expressions
  }

  bool boolop1() { return term("=="); }
  bool boolop2() { return term("!="); }
  bool boolop()
  {
    //TODO: next and run expressions
  }

  bool boolval1() { return term("false"); }
  bool boolval2() { return term("true"); }
  bool boolval()
  {
    //TODO: next and run expressions
  }

  bool intop1() { return term("+"); }
  bool intop()
  {
    //TODO: next and run expressions
  }