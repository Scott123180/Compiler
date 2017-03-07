

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

  bool term(string tt) //terminal
  {
    bool validTypeOrTerminal;
    validTypeOrTerminal = ((Token::tokens[(i)].getType() == tt)
                        ||((Token::tokens[(i)]).getValue() == tt));
    ++i;
    return validTypeOrTerminal;
  }

  bool Program1() { return Block() && term("$"); }

  bool Program()
  {
    int save = i; return (i = save, Program1());
  }

  bool Block1() { return term("{") && StatementList() && term("}"); }
  bool Block2() { return true; }//epsilon

  bool Block()
  {
    int save = i; return (i = save, Block1())
                       ||(i = save, Block2());
  }

  bool Statement1() { return PrintStatement(); }
  bool Statement2() { return AssignmentStatment(); }
  bool Statement3() { return VarDecl(); }
  bool Statement4() { return WhileStatement(); }
  bool Statement5() { return IfStatement(); }
  bool Statement6() { return Block(); }

  bool Statement()
  {
    int save = i; return (i = save, Statement1())
                       ||(i = save, Statement2())
                       ||(i = save, Statement3())
                       ||(i = save, Statement4())
                       ||(i = save, Statement5())
                       ||(i = save, Statement6());
  }

  bool PrintStatement1(){ return term("print") && term("(") && Expr() && term(")"); }

  bool PrintStatement()
  {
    int save = i; return (i = save, PrintStatement1());
  }

  bool AssignmentStatement1() { return Id() && term("=") && Expr(); }

  bool AssignmentStatement()
  {
    int save = i; return (i = save, AssignmentStatement1());
  }

  bool VarDecl1() { return type() && Id(); }
  bool VarDecl()
  {
    int save = i; return (i = save, VarDecl1());
  }

  bool WhileStatement1() { return term("while") && BooleanExpr() && Block(); }

  bool WhileStatement()
  {
    int save = i; return (i = save, WhileStatement1());
  }

  bool IfStatement1() { return term("if") && BooleanExpr() && Block(); }
  bool IfStatement()
  {
    int save = i; return ( i = save, IfStatement1());
  }

  bool Expr1() { return IntExpr(); }
  bool Expr2() { return StringExpr(); }
  bool Expr3() { return BooleanExpr(); }
  bool Expr4() { return Id(); }

  bool Expr()
  {
   int save = i; return ( i = save, Expr1())
                      ||( i = save, Expr2())
                      ||( i = save, Expr3())
                      ||( i = save, Expr4());
  }

  bool IntExpr1() { return digit() && intop && Expr(); }
  bool IntExpr2() { return digit(); }

  bool IntExpr()
  {
    int save = i; return ( i = save, IntExpr1())
                       ||( i = save, IntExpr2());
  }

  bool StringExpr1() { return term("\"") && CharList() && term("\""); }
  bool StringExpr()
  {
    int save = i; return ( i = save, StringExpr1());
  }

  bool BooleanExpr1() { return term("(") && Expr() && boolop() && Expr(); }
  bool BooleanExpr2() { return boolval(); }
  bool BooleanExpr()
  {
    int save = i; return ( i = save, BooleanExpr1())
                       ||( i = save, BooleanExpr2());
  }

  bool Id1() { return Char(); }
  bool Id()
  {
    int save = i; return ( i = save, Id1());
  }

  bool CharList1() { return Char() && CharList(); }
  bool CharList2() { return space() && CharList(); }
  bool CharList3() { return true; }
  bool CharList()
  {
    int save = i; return ( i = save, CharList1())
                       ||( i = save, CharList2())
                       ||( i = save, CharList3());
  }

  bool type1() { term("int"); }
  bool type2() { term("string"); }
  bool type3() { term("boolean"); }
  bool type()
  {
    int save = i; return ( i = save, type1())
                       ||( i = save, type2())
                       ||( i = save, type3());
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
    int save = i; return ( i = save, Char1());
  }

  bool space1() { return term(" "); }
  bool space()
  {
    int save = i; return ( i = save, space1());
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
    int save = i; return ( i = save, digit1());
  }

  bool boolop1() { return term("=="); }
  bool boolop2() { return term("!="); }
  bool boolop()
  {
    int save = i; return ( i = save, boolop1())
                       ||( i = save, boolop2());
  }

  bool boolval1() { return term("false"); }
  bool boolval2() { return term("true"); }
  bool boolval()
  {
    int save = i; return ( i = save, boolval1())
                       ||( i = save, boolval2());
  }

  bool intop1() { return term("+"); }
  bool intop()
  {
    int save = i; return ( i = save, intop1());
  }