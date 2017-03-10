

#include "parser.h"
#include "token.h"
#include "cst.h"

Parser::Parser(vector<Token> stream)
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
  Parser::tokens = stream;
  
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
      Parser::newCST().addChild(Parser::newCST().curNode, false);
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
    Parser::newCST().addChild(newBranch, true);
    if(Block())
    {
      if(term("EOP"))
      {
        Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
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
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
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
      Parser::newCST().addChild(newBranch, true);
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
      else //fail StatementList()
      {
        expecting.push_back("StatementList()");
        Parser::newCST().curNode = newBranch->parent; //kick back pointer
        Parser::newCST().deleteNode(newBranch, false);
        return false;
      }
    }
    else //fail leftBrace
    {
      expecting.push_back("{");
      return false;
    }
  }
  bool Parser::Block2()
  {
    expecting.clear();
    return true;
  }//epsilon

  bool Parser::Block()
  {
    int save = Parser::i; return (Parser::i = save, Block1())
                       ||(Parser::i = save, Block2());
  }

  //STATEMENTLIST=========================================================

  bool Parser::StatementList1() //Statement() StatementList()
  {
    Token* newBranchS = new Token("Statement");
    Parser::newCST().addChild(newBranchS, true);
    if (Statement())
    {
      Token* newBranchSL = new Token ("StatementList");
      Parser::newCST().addChild(newBranchSL,true);
      if (StatementList())
      {
        expecting.clear();
        Parser::newCST().curNode = newBranchSL->parent; //kick back pointer
        return true;
      }
      else //StatementList()
      {
        expecting.push_back("StatementList()");
        Parser::newCST().curNode = newBranchSL->parent; //kick back pointer
        Parser::newCST().deleteNode(newBranchS, false);
        return false;
      }
    }
    else //Statement()
    {
      expecting.push_back("Statement()");
      Parser::newCST().curNode = newBranchS->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranchS, false);
      return false;
    }
  }
  bool Parser::StatementList2() //epsilon
  {
    expecting.clear();
    return true;
  }

  bool Parser::StatementList()
  {
    int save = Parser::i; return (Parser::i = save, StatementList1())
                               ||(Parser::i = save, StatementList2());
  }

  //STATEMENT=============================================================

  bool Parser::Statement1() //PrintStatement()
  {
    Token* newBranch = new Token("PrintStatement");
    Parser::newCST().addChild(newBranch, true);
    if(PrintStatement())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //PrintStatement()
    {
      expecting.push_back("PrintStatement()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
      return false;
    }
  }
  bool Parser::Statement2() //AssignStatement()
  {
    Token* newBranch = new Token("AssignStatement");
    Parser::newCST().addChild(newBranch, true);
    if(AssignmentStatement())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //AssignmentStatement()
    {
      expecting.push_back("AssignmentStatement()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
      return false;
    }
  }
  bool Parser::Statement3() //VarDecl()
  {
    Token* newBranch = new Token("VarDecl");
    Parser::newCST().addChild(newBranch, true);
    if(VarDecl())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //VarDecl()
    {
      expecting.push_back("VarDecl()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
      return false;
    }
  }
  bool Parser::Statement4() //WhileStatement()
  {
    Token* newBranch = new Token("WhileStatement");
    Parser::newCST().addChild(newBranch, true);
    if(WhileStatement())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //WhileStatement()
    {
      expecting.push_back("WhileStatement()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
      return false;
    }
  }
  bool Parser::Statement5() //IfStatement()
  {
    Token* newBranch = new Token("IfStatement");
    Parser::newCST().addChild(newBranch, true);
    if(IfStatement())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //IfStatement()
    {
      expecting.push_back("IfStatement()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
      return false;
    }
  }
  bool Parser::Statement6() //Block()
  {
    Token* newBranch = new Token("Block");
    Parser::newCST().addChild(newBranch, true);
    if(Block())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //Block()
    {
      expecting.push_back("BlockStatement()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
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
        Parser::newCST().addChild(newBranch, true);
        if (Expr())
        {
          if (term("rightParen"))
          {
            expecting.clear();
            Parser::newCST().curNode = newBranch->parent; //kick back pointer
            return true;
          }
          else //rightParen
          {
            expecting.push_back("(");
            Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
            return false;
          }
        }
        else  //Expr()
        {
          expecting.push_back("Expr()");
          Parser::newCST().curNode = newBranch->parent; //kick back pointer
          Parser::newCST().deleteNode(newBranch, false);
          return false;
        }
      }
      else //leftParen
      {
        expecting.push_back("(");
        Parser::newCST().curNode = Parser::newCST().curNode->parent;
        return false;
      }
    }
    else //print
    {
      expecting.push_back("print");
      Parser::newCST().curNode = Parser::newCST().curNode->parent;
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
    Parser::newCST().addChild(newBranchI, true);
    if(Id())
    {
      if(term("="))
      {
        Token* newBranchE = new Token("Expr");
        Parser::newCST().addChild(newBranchE, true);
        if(Expr())
        {
          expecting.clear();
          Parser::newCST().curNode = newBranchE->parent; //kick back pointer
          return true;
        }
        else //Expr()
        {
          expecting.push_back("Expr()");
          Parser::newCST().curNode = newBranchE->parent; //kick back pointer
          Parser::newCST().deleteNode(newBranchE, false);
          return false;
        }
      }
      else //=
      {
        expecting.push_back("=");
        Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
        return false;
      }
    }
    else //Id()
    {
      expecting.push_back("Id()");
      Parser::newCST().curNode = newBranchI->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranchI, false);
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
    Parser::newCST().addChild(newBranchT, true);
    if(type())
    {
      Token* newBranchI = new Token("Id");
      Parser::newCST().addChild(newBranchI, true);
      if(Id())
      {
        expecting.clear();
        Parser::newCST().curNode = newBranchI->parent; //kick back pointer
        return true;
      }
      else //Id()
      {
        expecting.push_back("Id()");
        Parser::newCST().curNode = newBranchI->parent; //kick back pointer
        Parser::newCST().deleteNode(newBranchI, false);
        return false;
      }
    }
    else //type()
    {
      expecting.push_back("Type()");
      Parser::newCST().curNode = newBranchT->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranchT, false);
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
      Parser::newCST().addChild(newBranchBool, true);
      if(BooleanExpr())
      {
        Token* newBranchBlock = new Token("Block");
        Parser::newCST().addChild(newBranchBlock, true);
        if(Block())
        {
          expecting.clear();
          Parser::newCST().curNode = newBranchBlock->parent; //kick back pointer
          return true;
        }
        else //Block()
        {
          expecting.push_back("Block()");
          Parser::newCST().curNode = newBranchBlock->parent; //kick back pointer
          Parser::newCST().deleteNode(newBranchBlock, false);
          return false;
        }
      }
      else //BooleanExpr()
      {
        expecting.push_back("BooleanExpr()");
        Parser::newCST().curNode = newBranchBool->parent; //kick back pointer
        Parser::newCST().deleteNode(newBranchBool, false);
        return false;
      }
    }
    else //while
    {
      expecting.push_back("while");
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
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
      Parser::newCST().addChild(newBranchBool, true);
      if(BooleanExpr())
      {
        Token* newBranchBlock = new Token("Block");
        Parser::newCST().addChild(newBranchBlock, true);
        if(Block())
        {
          expecting.clear();
          Parser::newCST().curNode = newBranchBlock->parent; //kick back pointer
          return true;
        }
        else //Block()
        {
          expecting.push_back("Block()");
          Parser::newCST().curNode = newBranchBlock->parent; //kick back pointer
          Parser::newCST().deleteNode(newBranchBlock, false);
          return false;
        }
      }
      else //BooleanExpr()
      {
        expecting.push_back("BooleanExpr()");
        Parser::newCST().curNode = newBranchBool->parent; //kick back pointer
        Parser::newCST().deleteNode(newBranchBool, false);
        return false;
      }
    }
    else //if
    {
      expecting.push_back("if");
      Parser::newCST().curNode = Parser::newCST().curNode->parent;
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
    Parser::newCST().addChild(newBranch, true);
    if(IntExpr())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //IntExpr()
    {
      expecting.push_back("IntExpr()");
      Parser::newCST().curNode = newBranch->parent;
      Parser::newCST().deleteNode(newBranch, false);
      return false;
    }
  }
  bool Parser::Expr2() //StringExpr()
  {
    Token* newBranch = new Token("StringExpr");
    Parser::newCST().addChild(newBranch, true);
    if(StringExpr())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //StringExpr()
    {
      expecting.push_back("StringExpr()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
      return false;
    }
  }
  bool Parser::Expr3() //BooleanExpr()
  {
    Token* newBranch = new Token("BooleanExpr");
    Parser::newCST().addChild(newBranch, true);
    if(BooleanExpr())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //BooleanExpr()
    {
      expecting.push_back("BooleanExpr()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
      return false;
    }
  }
  bool Parser::Expr4() //Id()
  {
    Token* newBranch = new Token("Id");
    Parser::newCST().addChild(newBranch, true);
    if(Id())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else//Id()
    {
      expecting.push_back("Id()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
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
    Parser::newCST().addChild(newBranchDigit, true);
    if(digit())
    {
      if(term("intop"))
      {
        Token* newBranchExpr = new Token("Expr");
        Parser::newCST().addChild(newBranchExpr, true);
         if(Expr())
         {
           expecting.clear();
           Parser::newCST().curNode = newBranchExpr->parent; //kick back pointer
           return true;
         }
         else //Expr()
         {
           expecting.push_back("Expr()");
           Parser::newCST().curNode = newBranchExpr->parent; //kick back pointer
           Parser::newCST().deleteNode(newBranchExpr, false);
           return false;
         }
      }
      else //intop
      {
        expecting.push_back("intop");
        Parser::newCST().curNode = Parser::newCST().curNode->parent;
        return false;
      }
    }
    else //digit()
    {
      expecting.push_back("Digit()");
      Parser::newCST().curNode = newBranchDigit->parent;
      Parser::newCST().deleteNode(newBranchDigit, false);
      return false;
    }
  }
  bool Parser::IntExpr2() //digit()
  {
    Token* newBranch = new Token("Digit");
    Parser::newCST().addChild(newBranch, true);
    if(digit())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //digit()
    {
      expecting.push_back("Digit()");
      Parser::newCST().curNode = newBranch->parent;
      Parser::newCST().deleteNode(newBranch, false);
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
      Parser::newCST().addChild(newBranch, true);
      if(CharList())
      {
        if(term("rightQuote"))
        {
          expecting.clear();
          Parser::newCST().curNode = newBranch->parent; //kick back pointer
          return true;
        }
        else //rightQuote
        {
          expecting.push_back("\"");
          Parser::newCST().curNode = Parser::newCST().curNode->parent;
          return false;
        }
      }
      else //CharList()
      {
        expecting.push_back("CharList()");
        Parser::newCST().curNode = newBranch->parent;
        Parser::newCST().deleteNode(newBranch, false);
        return false;
      }
    }
    else //leftQuote
    {
      expecting.push_back("\"");
      Parser::newCST().curNode = Parser::newCST().curNode->parent;
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
      Parser::newCST().addChild(newBranchExpr1, true);
      if(Expr())
      {
        Token* newBranchBool = new Token("BoolOp");
        Parser::newCST().addChild(newBranchBool, true);
        if (boolop())
        {
          Token* newBranchExpr2 = new Token("Expr");
          Parser::newCST().addChild(newBranchExpr2, true);
          if(Expr())
          {
            if(term("rightParen"))
            {
              expecting.clear();
              Parser::newCST().curNode = newBranchExpr2->parent; //kick back pointer
              return true;
            }
            else //rightParen
            {
              expecting.push_back(")");
              Parser::newCST().curNode = Parser::newCST().curNode->parent;
              return false;
            }
          }
          else //Expr() #2
          {
            expecting.push_back("Expr()");
            Parser::newCST().curNode = newBranchExpr2->parent;
            Parser::newCST().deleteNode(newBranchExpr2, false);
            return false;
          }
        }
        else //boolop()
        {
          expecting.push_back("Boolop()");
          Parser::newCST().curNode = newBranchBool->parent;
          Parser::newCST().deleteNode(newBranchBool, false);
          return false;
        }
      }
      else //Expr()
      {
        expecting.push_back("Expr()");
        Parser::newCST().curNode = newBranchExpr1->parent;
        Parser::newCST().deleteNode(newBranchExpr1, false);
        return false;
      }
    }
    else //leftParen
    {
      expecting.push_back("(");
      Parser::newCST().curNode = Parser::newCST().curNode->parent;
      return false;
    }
  }
  bool Parser::BooleanExpr2() //boolval()
  {
    Token* newBranch = new Token("BoolVal");
    Parser::newCST().addChild(newBranch, true);
    if(boolval())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //boolval()
    {
      expecting.push_back("Boolval()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
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
    Parser::newCST().addChild(newBranch, true);
    if(Char())
    {
      expecting.clear();
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //Char()
    {
      expecting.push_back("Char()");
      Parser::newCST().curNode = newBranch->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranch, false);
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
    Parser::newCST().addChild(newBranchChar, true);
    if(Char())
    {
      Token* newBranchCharL = new Token("CharList");
      Parser::newCST().addChild(newBranchCharL, true);
      if(CharList())
      {
        expecting.clear();
        Parser::newCST().curNode = newBranchCharL->parent; //kick back pointer
        return true;
      }
      else //CharList()
      {
        expecting.push_back("CharList()");
        Parser::newCST().curNode = newBranchCharL->parent; //kick back the pointer
        Parser::newCST().deleteNode(newBranchCharL, false);
        return false;
      }
    }
    else //Char()
    {
      expecting.push_back("Char()");
      Parser::newCST().curNode = newBranchChar->parent; //kick back pointer
      Parser::newCST().deleteNode(newBranchChar,false);
      return false;
    }
  }
  bool Parser::CharList2() //space() CharList()
  {
    Token* newBranchS = new Token("Space");
    Parser::newCST().addChild(newBranchS, true);
    if(space())
    {
      Token* newBranchCharL = new Token("CharList");
      Parser::newCST().addChild(newBranchCharL, true);
      if(CharList())
      {
        expecting.clear();
        Parser::newCST().curNode = newBranchCharL->parent; //kick back pointer
        return true;
      }
      else //CharList()
      {
        expecting.push_back("CharList()");
        Parser::newCST().curNode = newBranchCharL->parent;
        Parser::newCST().deleteNode(newBranchCharL, false);
        return false;
      }
    }
    else //space()
    {
      expecting.push_back("space()");
      Parser::newCST().curNode = newBranchS->parent;
      Parser::newCST().deleteNode(newBranchS, false);
      return false;
    }
  }
  bool Parser::CharList3() //epsilon
  {
    expecting.clear();
    return true;
  }
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
      expecting.clear();
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
      return true;
    }
    else //type
    {
      expecting.push_back("type");
      Parser::newCST().curNode = Parser::newCST().curNode->parent;
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
      expecting.clear();
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
      return true;
    }
    else //Char
    {
      expecting.push_back("Char");
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
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
      expecting.clear();
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
      return true;
    }
    else //space
    {
      expecting.push_back("space_char");
      Parser::newCST().curNode = Parser::newCST().curNode->parent;
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
      expecting.clear();
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
      return true;
    }
    else //digit
    {
      expecting.push_back("digit");
      Parser::newCST().curNode = Parser::newCST().curNode->parent;
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
      expecting.clear();
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
      return true;
    }
    else //boolop
    {
      expecting.push_back("boolop");
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
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
      expecting.clear();
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
      return true;
    }
    else //boolval
    {
      expecting.push_back("boolval");
      Parser::newCST().curNode = Parser::newCST().curNode->parent;
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
      expecting.clear();
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
      return true;
    }
    else //intop
    {
      expecting.push_back("intop");
      Parser::newCST().curNode = Parser::newCST().curNode->parent; //kick back pointer
      return false;
    }
  }
  bool Parser::intop()
  {
    int save = Parser::i; return ( Parser::i = save, intop1());
  }