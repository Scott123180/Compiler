

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

  //remember, if a process fails, we need to delete all of the children
  
  Program();



}
/*

 bool term(TOKEN tok) {return *next++ == tok; }
   returns true if the token we passed in matches the input or no it doesn't
 */

  bool Parser::term(string tt) //terminal leaf creation
  {
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


  //PROGRAM===============================================================
  //already added a program token as our root node
  bool Parser::Program1()
  {
    //create block branch
    Token* newBranch = new Token("Block");
    CST::addChild(newBranch, true);
    if(Block())
    {
      if(term("EOP"))
      {
        CST::curNode = CST::curNode->parent; //kick back pointer
        expecting.clear();
        return true;
      }
      else
      {
        expecting.push_back("$");
        return false;
      }
    }
    else
    {
      expecting.push_back("Block()");
      CST::curNode = newBranch->parent; //kick back pointer
      CST::deleteNode(newBranch, false);
      return false;
    }
  }

  bool Parser::Program()
  {
    int save = Parser::i; return (Parser::i = save, Program1());
  }

  //BLOCK=================================================================

  bool Parser::Block1()  //leftBrace, StatementList(), rightBrace
  {
    if(term("leftBrace"))
    {
      Token* newBranch = new Token("StatementList"); //add token preemtively
      CST::addChild(newBranch, true);
      if(StatementList())
      {
        if(term("rightBrace"))
        {
          expecting.clear();
          return true;
        }
        else //fail rightBrace
        {
          expecting.push_back("}");
          return false;
        }
      }
      else //fail StatementList
      {
        expecting.push_back("StatementList()");
        CST::curNode = newBranch->parent; //kick back pointer
        CST::deleteNode(newBranch, false);
        return false;
      }
    }
    else //fail leftBrace
    {
      expecting.push_back("{");
      return false;
    }
  }
  bool Parser::Block2() { return true; }//epsilon

  bool Parser::Block()
  {
    int save = Parser::i; return (Parser::i = save, Block1())
                       ||(Parser::i = save, Block2());
  }

  //STATEMENTLIST=========================================================

  bool Parser::StatementList1() {
    Token* newBranchS = new Token("Statement");
    CST::addChild(newBranchS, true);
    if (Statement())
    {
      Token* newBranchSL = new Token ("StatementList");
      CST::addChild(newBranchSL,true);
      if (StatementList())
      {
        expecting.clear();
        CST::curNode = newBranchSL->parent; //kick back pointer
        return true;
      }
      else
      {
        expecting.push_back("StatementList()");
        CST::curNode = newBranchSL->parent; //kick back pointer
        CST::deleteNode(newBranchS, false);
        return false;
      }
    }
    else
    {
      expecting.push_back("Statement()");
      CST::curNode = newBranchS->parent; //kick back pointer
      CST::deleteNode(newBranchS, false);
      return false;
    }
  }
  bool Parser::StatementList2() { return true; } //epsilon

  bool Parser::StatementList()
  {
    int save = Parser::i; return (Parser::i = save, StatementList1())
                               ||(Parser::i = save, StatementList2());
  }

  //STATEMENT=============================================================

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

  //PRINTSTATEMENT========================================================

  bool Parser::PrintStatement1(){ return term("print") && term("leftParen") && Expr() && term("rightParen"); }

  bool Parser::PrintStatement()
  {
    int save = Parser::i; return (Parser::i = save, PrintStatement1());
  }

  //ASSIGNMENTSTATEMENT====================================================

  bool Parser::AssignmentStatement1() { return Id() && term("=") && Expr(); }

  bool Parser::AssignmentStatement()
  {
    int save = Parser::i; return (Parser::i = save, AssignmentStatement1());
  }

  //VARDECL===============================================================

  bool Parser::VarDecl1() { return type() && Id(); }
  bool Parser::VarDecl()
  {
    int save = Parser::i; return (Parser::i = save, VarDecl1());
  }

  //WHILESTATEMENT========================================================

  bool Parser::WhileStatement1() { return term("while") && BooleanExpr() && Block(); }

  bool Parser::WhileStatement()
  {
    int save = Parser::i; return (Parser::i = save, WhileStatement1());
  }

  //IFSTATEMENT===========================================================

  bool Parser::IfStatement1() { return term("if") && BooleanExpr() && Block(); }
  bool Parser::IfStatement()
  {
    int save = Parser::i; return ( Parser::i = save, IfStatement1());
  }

  //EXPR==================================================================

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

  //INTEXPR===============================================================

  bool Parser::IntExpr1() { return digit() && intop && Expr(); }
  bool Parser::IntExpr2() { return digit(); }

  bool Parser::IntExpr()
  {
    int save = Parser::i; return ( Parser::i = save, IntExpr1())
                       ||( Parser::i = save, IntExpr2());
  }

  //STRINGEXPR============================================================

  bool Parser::StringExpr1() { return term("leftQuote") && CharList() && term("rightQuote"); }
  bool Parser::StringExpr()
  {
    int save = Parser::i; return ( Parser::i = save, StringExpr1());
  }

  //BOOLEANEXPR===========================================================

  bool Parser::BooleanExpr1() { return term("leftParen") && Expr() && boolop() && Expr() && term("rightParen"); }
  bool Parser::BooleanExpr2() { return boolval(); }
  bool Parser::BooleanExpr()
  {
    int save = Parser::i; return ( Parser::i = save, BooleanExpr1())
                       ||( Parser::i = save, BooleanExpr2());
  }

  //ID====================================================================

  bool Parser::Id1() { return Char(); }
  bool Parser::Id()
  {
    int save = Parser::i; return ( Parser::i = save, Id1());
  }

  //CHARLIST==============================================================

  bool Parser::CharList1() { return Char() && CharList(); }
  bool Parser::CharList2() { return space() && CharList(); }
  bool Parser::CharList3() { return true; }
  bool Parser::CharList()
  {
    int save = Parser::i; return ( Parser::i = save, CharList1())
                       ||( Parser::i = save, CharList2())
                       ||( Parser::i = save, CharList3());
  }

  //TYPE==================================================================

  bool Parser::type1() { term("type"); }
  bool Parser::type()
  {
    int save = Parser::i; return ( Parser::i = save, type1());
  }

  //CHAR==================================================================

  bool Parser::Char1() { return term("Char");  }
  bool Parser::Char()
  {
    int save = Parser::i; return ( Parser::i = save, Char1());
  }

  //SPACE=================================================================

  bool Parser::space1() { return term(" "); }
  bool Parser::space()
  {
    int save = Parser::i; return ( Parser::i = save, space1());
  }

  //DIGIT=================================================================

  bool Parser::digit1() { return term("digit"); }
  bool Parser::digit()
  {
    int save = Parser::i; return ( Parser::i = save, digit1());
  }

  //BOOLOP================================================================

  bool Parser::boolop1() { return term("boolOp"); }
  bool Parser::boolop()
  {
    int save = Parser::i; return ( Parser::i = save, boolop1());
  }

  //BOOLVAL===============================================================

  bool Parser::boolval1() { return term("boolVal"); }
  bool Parser::boolval()
  {
    int save = Parser::i; return ( Parser::i = save, boolval1());
  }

  //INTOP=================================================================

  bool Parser::intop1() { return term("intOp"); }
  bool Parser::intop()
  {
    int save = Parser::i; return ( Parser::i = save, intop1());
  }