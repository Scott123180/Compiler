

#include "parser.h"
#include "token.h"
#include "cst.h"

Parser::Parser(vector<Token> stream)
  : tokens(stream)
{

  /*
   * next is the current token we're working with, we increment it every time just
   * in case we find a match, but then set it back if we have to check again if a term
   * matches
   *
   * term checks if something matches a term
   *
   * if there is a match in the function term, then create a leaf, otherwise return
   *
   * in each branch check, we must add each branch and delete it if it does not work
   */
  //initialize next to point to the first token
  //invoke E()

  Program();



}
/*

 bool term(TOKEN tok) {return *next++ == tok; }
   returns true if the token we passed in matches the input or no it doesn't
 */

  bool Parser::term(string tt) //terminal leaf creation
  {
    //TODO: CST and token functions here on match

    if (Parser::tokens[(Parser::i++)].getType() == tt)
    {
      //create leaf
      CST::addChild(CST::curNode, false);
      return true;
    }
    else //no match
    {
      return false;
    }
  }


  //already added a program token as our root node
  bool Parser::Program1()
  {
    //create block branch
    Token* newBranch = new Token("Block");
    CST::addChild(newBranch, true);
    if(Block() && term("EOP"))
    {
      return true;
      //kick pointer back to root
    }
    else
    {
      //TODO: add to error statements
      return false;
    }
  }

  bool Parser::Program()
  {
    int save = Parser::i; return (Parser::i = save, Program1());
  }

  bool Parser::Block1() { return term("leftBrace") && StatementList() && term("rightBrace"); }
  bool Parser::Block2() { return true; }//epsilon

  bool Parser::Block()
  {
    int save = Parser::i; return (Parser::i = save, Block1())
                       ||(Parser::i = save, Block2());
  }

  bool Parser::StatementList1() { return Statement() && StatementList(); }
  bool Parser::StatementList2() { return true; } //epsilon

  bool Parser::StatementList()
  {
    int save = Parser::i; return (Parser::i = save, StatementList1())
                               ||(Parser::i = save, StatementList2());
  }

  bool Parser::Statement1() { return PrintStatement(); }
  bool Parser::Statement2() { return AssignmentStatement(); }
  bool Parser::Statement3() { return VarDecl(); }
  bool Parser::Statement4() { return WhileStatement(); }
  bool Parser::Statement5() { return IfStatement(); }
  bool Parser::Statement6() { return Block(); }

  bool Parser::Statement()
  {
    int save = Parser::i; return (Parser::i = save, Statement1())
                       ||(Parser::i = save, Statement2())
                       ||(Parser::i = save, Statement3())
                       ||(Parser::i = save, Statement4())
                       ||(Parser::i = save, Statement5())
                       ||(Parser::i = save, Statement6());
  }

  bool Parser::PrintStatement1(){ return term("print") && term("leftParen") && Expr() && term("rightParen"); }

  bool Parser::PrintStatement()
  {
    int save = Parser::i; return (Parser::i = save, PrintStatement1());
  }

  bool Parser::AssignmentStatement1() { return Id() && term("=") && Expr(); }

  bool Parser::AssignmentStatement()
  {
    int save = Parser::i; return (Parser::i = save, AssignmentStatement1());
  }

  bool Parser::VarDecl1() { return type() && Id(); }
  bool Parser::VarDecl()
  {
    int save = Parser::i; return (Parser::i = save, VarDecl1());
  }

  bool Parser::WhileStatement1() { return term("while") && BooleanExpr() && Block(); }

  bool Parser::WhileStatement()
  {
    int save = Parser::i; return (Parser::i = save, WhileStatement1());
  }

  bool Parser::IfStatement1() { return term("if") && BooleanExpr() && Block(); }
  bool Parser::IfStatement()
  {
    int save = Parser::i; return ( Parser::i = save, IfStatement1());
  }

  bool Parser::Expr1() { return IntExpr(); }
  bool Parser::Expr2() { return StringExpr(); }
  bool Parser::Expr3() { return BooleanExpr(); }
  bool Parser::Expr4() { return Id(); }

  bool Parser::Expr()
  {
   int save = Parser::i; return ( Parser::i = save, Expr1())
                      ||( Parser::i = save, Expr2())
                      ||( Parser::i = save, Expr3())
                      ||( Parser::i = save, Expr4());
  }

  bool Parser::IntExpr1() { return digit() && intop && Expr(); }
  bool Parser::IntExpr2() { return digit(); }

  bool Parser::IntExpr()
  {
    int save = Parser::i; return ( Parser::i = save, IntExpr1())
                       ||( Parser::i = save, IntExpr2());
  }

  bool Parser::StringExpr1() { return term("leftQuote") && CharList() && term("rightQuote"); }
  bool Parser::StringExpr()
  {
    int save = Parser::i; return ( Parser::i = save, StringExpr1());
  }

  bool Parser::BooleanExpr1() { return term("leftParen") && Expr() && boolop() && Expr() && term("rightParen"); }
  bool Parser::BooleanExpr2() { return boolval(); }
  bool Parser::BooleanExpr()
  {
    int save = Parser::i; return ( Parser::i = save, BooleanExpr1())
                       ||( Parser::i = save, BooleanExpr2());
  }

  bool Parser::Id1() { return Char(); }
  bool Parser::Id()
  {
    int save = Parser::i; return ( Parser::i = save, Id1());
  }

  bool Parser::CharList1() { return Char() && CharList(); }
  bool Parser::CharList2() { return space() && CharList(); }
  bool Parser::CharList3() { return true; }
  bool Parser::CharList()
  {
    int save = Parser::i; return ( Parser::i = save, CharList1())
                       ||( Parser::i = save, CharList2())
                       ||( Parser::i = save, CharList3());
  }

  bool Parser::type1() { term("type"); }
  bool Parser::type()
  {
    int save = Parser::i; return ( Parser::i = save, type1());
  }

  bool Parser::Char1() { return term("Char");  }
  bool Parser::Char()
  {
    int save = Parser::i; return ( Parser::i = save, Char1());
  }

  bool Parser::space1() { return term(" "); }
  bool Parser::space()
  {
    int save = Parser::i; return ( Parser::i = save, space1());
  }

  bool Parser::digit1() { return term("digit"); }
  bool Parser::digit()
  {
    int save = Parser::i; return ( Parser::i = save, digit1());
  }

  bool Parser::boolop1() { return term("boolOp"); }
  bool Parser::boolop()
  {
    int save = Parser::i; return ( Parser::i = save, boolop1());
  }

  bool Parser::boolval1() { return term("boolVal"); }
  bool Parser::boolval()
  {
    int save = Parser::i; return ( Parser::i = save, boolval1());
  }

  bool Parser::intop1() { return term("intOp"); }
  bool Parser::intop()
  {
    int save = Parser::i; return ( Parser::i = save, intop1());
  }