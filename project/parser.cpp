

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
      else //EOP
      {
        expecting.push_back("$");
        return false;
      }
    }
    else //Block()
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

  bool Parser::Statement1() //PrintStatement()
  {
    Token* newBranch = new Token("PrintStatement");
    CST::addChild(newBranch, true);
    if(PrintStatement())
    {
      return true;
    }
    else //PrintStatement()
    {
      return false;
    }
  }
  bool Parser::Statement2() //AssignStatement()
  {
    Token* newBranch = new Token("AssignStatement");
    CST::addChild(newBranch, true);
    if(AssignmentStatement())
    {
      return true;
    }
    else //AssignmentStatement()
    {
      return false;
    }
  }
  bool Parser::Statement3() //VarDecl()
  {
    Token* newBranch = new Token("VarDecl");
    CST::addChild(newBranch, true);
    if(VarDecl())
    {
      return true;
    }
    else //VarDecl()
    {
      return false;
    }
  }
  bool Parser::Statement4() //WhileStatement()
  {
    Token* newBranch = new Token("WhileStatement");
    CST::addChild(newBranch, true);
    if(WhileStatement())
    {
      return true;
    }
    else //WhileStatement()
    {
      return false;
    }
  }
  bool Parser::Statement5() //IfStatement()
  {
    Token* newBranch = new Token("IfStatement");
    CST::addChild(newBranch, true);
    if(IfStatement())
    {
      return true;
    }
    else //IfStatement()
    {
      return false;
    }
  }
  bool Parser::Statement6() //Block()
  {
    Token* newBranch = new Token("Block");
    CST::addChild(newBranch, true);
    if(Block())
    {
      return true;
    }
    else //Block()
    {
      return false;
    }
  }

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

  bool Parser::PrintStatement1() //print leftParen Expr() rightParen
  {
    if (term("print"))
    {
      if (term("leftParen"))
      {
        Token* newBranch = new Token("Expr");
        CST::addChild(newBranch, true);
        if (Expr())
        {
          if (term("rightParen"))
          {
            return true;
          }
          else //rightParen
          {
            return false;
          }
        }
        else  //Expr()
        {
          return false;
        }
      }
      else //leftParen
      {
        return false;
      }
    }
    else //print
    {
      return false;
    }
  }

  bool Parser::PrintStatement()
  {
    int save = Parser::i; return (Parser::i = save, PrintStatement1());
  }

  //ASSIGNMENTSTATEMENT====================================================

  bool Parser::AssignmentStatement1()  //Id = Expr()
  {
    Token* newBranchI = new Token("Id");
    CST::addChild(newBranchI, true);
    if(Id())
    {
      if(term("="))
      {
        Token* newBranchE = new Token("Expr");
        CST::addChild(newBranchE, true);
        if(Expr())
        {
          return true;
        }
        else //Expr()
        {
          return false;
        }
      }
      else //=
      {
        return false;
      }
    }
    else //Id()
    {
      return false;
    }
    
  }

  bool Parser::AssignmentStatement()
  {
    int save = Parser::i; return (Parser::i = save, AssignmentStatement1());
  }

  //VARDECL===============================================================

  bool Parser::VarDecl1()
  {
    Token* newBranchT = new Token("VarDecl");
    CST::addChild(newBranchT, true);
    if(type())
    {
      Token* newBranchI = new Token("Id");
      CST::addChild(newBranchI, true);
      if(Id())
      {
        return true;
      }
      else //Id()
      {
        return false;
      }
    }
    else //type()
    {
      return false;
    }
  }
  bool Parser::VarDecl()
  {
    int save = Parser::i; return (Parser::i = save, VarDecl1());
  }

  //WHILESTATEMENT========================================================

  bool Parser::WhileStatement1()  //while BoolExpr() Block()
  {
    if(term("while"))
    {
      Token* newBranchBool = new Token("BooleanExpr");
      CST::addChild(newBranchBool, true);
      if(BooleanExpr())
      {
        Token* newBranchBlock = new Token("Block");
        CST::addChild(newBranchBlock, true);
        if(Block())
        {
          return true;
        }
        else //Block()
        {
          return false;
        }
      }
      else //BooleanExpr()
      {
        return false;
      }
    }
    else //while
    {
      return false;
    }
  }

  bool Parser::WhileStatement()
  {
    int save = Parser::i; return (Parser::i = save, WhileStatement1());
  }

  //IFSTATEMENT===========================================================

  bool Parser::IfStatement1() //if BooleanExpr() Block()
  {
    if(term("if"))
    {
      Token* newBranchBool = new Token("BooleanExpr");
      CST::addChild(newBranchBool, true);
      if(BooleanExpr())
      {
        Token* newBranchBlock = new Token("Block");
        CST::addChild(newBranchBlock, true);
        if(Block())
        {
          return true;
        }
        else //Block()
        {
          return false;
        }
      }
      else //BooleanExpr()
      {
        
      }
    }
    else //if
    {
      return false;
    }
  }
  bool Parser::IfStatement()
  {
    int save = Parser::i; return ( Parser::i = save, IfStatement1());
  }

  //EXPR==================================================================

  bool Parser::Expr1()  //IntExpr()
  {
    Token* newBranch = new Token("IntExpr");
    CST::addChild(newBranch, true);
    if(IntExpr())
    {
      return true;
    }
    else //IntExpr()
    {
      return false;
    }
  }
  bool Parser::Expr2() //StringExpr()
  {
    Token* newBranch = new Token("StringExpr");
    CST::addChild(newBranch, true);
    if(StringExpr())
    {
      return true;
    }
    else //StringExpr()
    {
      return false;
    }
  }
  bool Parser::Expr3() //BooleanExpr()
  {
    Token* newBranch = new Token("BooleanExpr");
    CST::addChild(newBranch, true);
    if(BooleanExpr())
    {
      return true;
    }
    else //BooleanExpr()
    {
      return false;
    }
  }
  bool Parser::Expr4() //Id()
  {
    Token* newBranch = new Token("Id");
    CST::addChild(newBranch, true);
    if(Id())
    {
      return true;
    }
    else//Id()
    {
      return false;
    }
  }

  bool Parser::Expr()
  {
   int save = Parser::i; return ( Parser::i = save, Expr1())
                      ||( Parser::i = save, Expr2())
                      ||( Parser::i = save, Expr3())
                      ||( Parser::i = save, Expr4());
  }

  //INTEXPR===============================================================

  bool Parser::IntExpr1() //digit() intop Expr()
  {
    Token* newBranchDigit = new Token("Digit");
    CST::addChild(newBranchDigit, true);
    if(digit())
    {
      if(intop)
      {
        Token* newBranchExpr = new Token("Expr");
        CST::addChild(newBranchExpr, true);
         if(Expr())
         {
           return true;
         }
         else //Expr()
         {
           return false;
         }
      }
      else //intop
      {
        return false;
      }
    }
    else //digit()
    {
      return false;
    }
  }
  bool Parser::IntExpr2() //digit()
  {
    Token* newBranch = new Token("Digit");
    CST::addChild(newBranch, true);
    if(digit())
    {
      return true;
    }
    else //digit()
    {
      return false;
    }
  }

  bool Parser::IntExpr()
  {
    int save = Parser::i; return ( Parser::i = save, IntExpr1())
                       ||( Parser::i = save, IntExpr2());
  }

  //STRINGEXPR============================================================

  bool Parser::StringExpr1() //leftQuote CharList() rightQuote
  {
    if(term("leftQuote"))
    {
      Token* newBranch = new Token("CharList");
      CST::addChild(newBranch, true);
      if(CharList())
      {
        if(term("rightQuote"))
        {
          return true;
        }
        else //rightQuote
        {
          return false;
        }
      }
      else //CharList()
      {
        return false;
      }
    }
    else //leftQuote
    {
      return false;
    }
  }
  bool Parser::StringExpr()
  {
    int save = Parser::i; return ( Parser::i = save, StringExpr1());
  }

  //BOOLEANEXPR===========================================================

  bool Parser::BooleanExpr1() //leftParen Expr() boolop() Expr() rightParen
  {
    if(term("leftParen"))
    {
      Token* newBranchExpr1 = new Token("Expr");
      CST::addChild(newBranchExpr1, true);
      if(Expr())
      {
        Token* newBranchBool = new Token("BoolOp");
        CST::addChild(newBranchBool, true);
        if (boolop())
        {
          Token* newBranchExpr2 = new Token("Expr");
          CST::addChild(newBranchExpr2, true);
          if(Expr())
          {
            if(term("rightParen"))
            {
              return true;
            }
            else //rightParen
            {
              return false;
            }
          }
          else //Expr()
          {
            return false;
          }
        }
        else //boolop()
        {
          return false;
        }
      }
      else //Expr()
      {
        return false;
      }
    }
    else //leftParen
    {
      return false;
    }
  }
  bool Parser::BooleanExpr2() //boolval()
  {
    Token* newBranch = new Token("BoolVal");
    CST::addChild(newBranch, true);
    if(boolval())
    {
      return true;
    }
    else //boolval()
    {
      return false;
    }
  }
  bool Parser::BooleanExpr()
  {
    int save = Parser::i; return ( Parser::i = save, BooleanExpr1())
                       ||( Parser::i = save, BooleanExpr2());
  }

  //ID====================================================================

  bool Parser::Id1() //Char()
  {
    Token* newBranch = new Token("Char");
    CST::addChild(newBranch, true);
    if(Char())
    {
      return true;
    }
    else //Char()
    {
      return false;
    }
  }
  bool Parser::Id()
  {
    int save = Parser::i; return ( Parser::i = save, Id1());
  }

  //CHARLIST==============================================================

  bool Parser::CharList1() //Char() CharList()
  {
    Token* newBranchChar = new Token("Char");
    CST::addChild(newBranchChar, true);
    if(Char())
    {
      Token* newBranchCharL = new Token("CharList");
      CST::addChild(newBranchCharL, true);
      if(CharList())
      {
        return true;
      }
      else //CharList()
      {
        return false;
      }
    }
    else //Char()
    {
      return false;
    }
  }
  bool Parser::CharList2() //space() CharList()
  {
    Token* newBranchS = new Token("Space");
    CST::addChild(newBranchS, true);
    if(space())
    {
      Token* newBranchCharL = new Token("CharList");
      CST::addChild(newBranchCharL, true);
      if(CharList())
      {
        return true;
      }
      else //CharList()
      {
        return false;
      }
    }
    else //space()
    {
      return false;
    }
  }
  bool Parser::CharList3() { return true; } //epsilon
  bool Parser::CharList()
  {
    int save = Parser::i; return ( Parser::i = save, CharList1())
                       ||( Parser::i = save, CharList2())
                       ||( Parser::i = save, CharList3());
  }

  //TYPE==================================================================

  bool Parser::type1() //type
  {
    if(term("type"))
    {
      return true;
    }
    else //type
    {
      return false;
    }
  }
  bool Parser::type()
  {
    int save = Parser::i; return ( Parser::i = save, type1());
  }

  //CHAR==================================================================

  bool Parser::Char1()
  {
    if(term("Char"))
    {
      return true;
    }
    else //Char
    {
      return false;
    }
  }
  bool Parser::Char()
  {
    int save = Parser::i; return ( Parser::i = save, Char1());
  }

  //SPACE=================================================================

  bool Parser::space1()
  {
    if(term(" "))
    {
      return true;
    }
    else //space
    {
      return false;
    }
  }
  bool Parser::space()
  {
    int save = Parser::i; return ( Parser::i = save, space1());
  }

  //DIGIT=================================================================

  bool Parser::digit1()
  {
    if(term("digit"))
    {
      return true;
    }
    else //digit
    {
      return false;
    }
  }
  bool Parser::digit()
  {
    int save = Parser::i; return ( Parser::i = save, digit1());
  }

  //BOOLOP================================================================

  bool Parser::boolop1()
  {
    if(term("boolOp"))
    {
      return true;
    }
    else //boolop
    {
      return false;
    }
  }
  bool Parser::boolop()
  {
    int save = Parser::i; return ( Parser::i = save, boolop1());
  }

  //BOOLVAL===============================================================

  bool Parser::boolval1()
  {
    if(term("boolVal"))
    {
      return true;
    }
    else //boolval
    {
      return false;
    }
  }
  bool Parser::boolval()
  {
    int save = Parser::i; return ( Parser::i = save, boolval1());
  }

  //INTOP=================================================================

  bool Parser::intop1() {
    if(term("intOp"))
    {
      return true;
    }
    else //intop
    {
      return false;
    }
  }
  bool Parser::intop()
  {
    int save = Parser::i; return ( Parser::i = save, intop1());
  }