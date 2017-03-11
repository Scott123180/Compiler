#include <iostream>

#include "parser.h"
#include "token.h"
#include "cst.h"

using namespace std;

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
  
  if(Program())
  {
    cout << "Successful Parse!" << endl;
    
    
    //print out the cst in the command line
    //newCST.returnToRoot();  //go back to the root
    //newCST.dfio(newCST.curNode);
  }
  else
  {
    cout << "No segmentation faults, but the parse wasn't successful";
  }



}
/*

 bool term(TOKEN tok) {return *next++ == tok; }
   returns true if the token we passed in matches the input or no it doesn't
 */

  bool Parser::term(string tt) //terminal leaf creation
  {
    if (Parser::tokens[(Parser::i)].getType() == tt)
    {
      //put token on the heap
      Token* newTerminal = new Token(Parser::tokens[Parser::i]);
      //create leaf
      newCST.addChild(newTerminal, false);
      ++i; //increment i
      return true;
    }
    else //no match
    {
      ++i; //increment i regardless
      return false;
    }
  }


  //PROGRAM===============================================================
  //already added a program token as our root node
  bool Parser::Program1()
  {
    //create block branch
    Token* newBranch = new Token("Block");
    newCST.addChild(newBranch, true);
    if(Block())
    {
      if(term("EOP"))
      {
        newCST.curNode = newCST.curNode->parent; //kick back pointer to start
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
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
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
      newCST.addChild(newBranch, true);
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
        newCST.curNode = newBranch->parent; //kick back pointer
        newCST.deleteNode(newBranch);
        return false;
      }
    }
    else //fail leftBrace
    {
      expecting.push_back("{");
      return false;
    }
  }

  bool Parser::Block()
  {
    int save = Parser::i; return (Parser::i = save, Block1());
  }

  //STATEMENTLIST=========================================================

  bool Parser::StatementList1() //Statement() StatementList()
  {
    Token* newBranchS = new Token("Statement");
    newCST.addChild(newBranchS, true);
    if (Statement())
    {
      Token* newBranchSL = new Token ("StatementList");
      newCST.addChild(newBranchSL,true);
      if (StatementList())
      {
        expecting.clear();
        newCST.curNode = newBranchSL->parent; //kick back pointer
        return true;
      }
      else //StatementList()
      {
        expecting.push_back("StatementList()");
        newCST.curNode = newBranchSL->parent; //kick back pointer
        newCST.deleteNode(newBranchSL);
        return false;
      }
    }
    else //Statement()
    {
      expecting.push_back("Statement()");
      newCST.curNode = newBranchS->parent; //kick back pointer
      newCST.deleteNode(newBranchS);
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
    newCST.addChild(newBranch, true);
    if(PrintStatement())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //PrintStatement()
    {
      expecting.push_back("PrintStatement()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::Statement2() //AssignStatement()
  {
    Token* newBranch = new Token("AssignStatement");
    newCST.addChild(newBranch, true);
    if(AssignmentStatement())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //AssignmentStatement()
    {
      expecting.push_back("AssignmentStatement()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::Statement3() //VarDecl()
  {
    Token* newBranch = new Token("VarDecl");
    newCST.addChild(newBranch, true);
    if(VarDecl())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //VarDecl()
    {
      expecting.push_back("VarDecl()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::Statement4() //WhileStatement()
  {
    Token* newBranch = new Token("WhileStatement");
    newCST.addChild(newBranch, true);
    if(WhileStatement())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //WhileStatement()
    {
      expecting.push_back("WhileStatement()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::Statement5() //IfStatement()
  {
    Token* newBranch = new Token("IfStatement");
    newCST.addChild(newBranch, true);
    if(IfStatement())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //IfStatement()
    {
      expecting.push_back("IfStatement()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::Statement6() //Block()
  {
    Token* newBranch = new Token("Block");
    newCST.addChild(newBranch, true);
    if(Block())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //Block()
    {
      expecting.push_back("BlockStatement()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
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
        newCST.addChild(newBranch, true);
        if (Expr())
        {
          if (term("rightParen"))
          {
            expecting.clear();
            newCST.curNode = newBranch->parent; //kick back pointer
            return true;
          }
          else //rightParen
          {
            expecting.push_back("(");
            newCST.curNode = newCST.curNode->parent; //kick back pointer
            return false;
          }
        }
        else  //Expr()
        {
          expecting.push_back("Expr()");
          newCST.curNode = newBranch->parent; //kick back pointer
          newCST.deleteNode(newBranch);
          return false;
        }
      }
      else //leftParen
      {
        expecting.push_back("(");
        newCST.curNode = newCST.curNode->parent;
        return false;
      }
    }
    else //print
    {
      expecting.push_back("print");
      newCST.curNode = newCST.curNode->parent;
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
    newCST.addChild(newBranchI, true);
    if(Id())
    {
      if(term("="))
      {
        Token* newBranchE = new Token("Expr");
        newCST.addChild(newBranchE, true);
        if(Expr())
        {
          expecting.clear();
          newCST.curNode = newBranchE->parent; //kick back pointer
          return true;
        }
        else //Expr()
        {
          expecting.push_back("Expr()");
          newCST.curNode = newBranchE->parent; //kick back pointer
          newCST.deleteNode(newBranchE);
          return false;
        }
      }
      else //=
      {
        expecting.push_back("=");
        newCST.curNode = newCST.curNode->parent; //kick back pointer
        return false;
      }
    }
    else //Id()
    {
      expecting.push_back("Id()");
      newCST.curNode = newBranchI->parent; //kick back pointer
      newCST.deleteNode(newBranchI);
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
    newCST.addChild(newBranchT, true);
    if(type())
    {
      Token* newBranchI = new Token("Id");
      newCST.addChild(newBranchI, true);
      if(Id())
      {
        expecting.clear();
        newCST.curNode = newBranchI->parent; //kick back pointer
        return true;
      }
      else //Id()
      {
        expecting.push_back("Id()");
        newCST.curNode = newBranchI->parent; //kick back pointer
        newCST.deleteNode(newBranchI);
        return false;
      }
    }
    else //type()
    {
      expecting.push_back("Type()");
      newCST.curNode = newBranchT->parent; //kick back pointer
      newCST.deleteNode(newBranchT);
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
      newCST.addChild(newBranchBool, true);
      if(BooleanExpr())
      {
        Token* newBranchBlock = new Token("Block");
        newCST.addChild(newBranchBlock, true);
        if(Block())
        {
          expecting.clear();
          newCST.curNode = newBranchBlock->parent; //kick back pointer
          return true;
        }
        else //Block()
        {
          expecting.push_back("Block()");
          newCST.curNode = newBranchBlock->parent; //kick back pointer
          newCST.deleteNode(newBranchBlock);
          return false;
        }
      }
      else //BooleanExpr()
      {
        expecting.push_back("BooleanExpr()");
        newCST.curNode = newBranchBool->parent; //kick back pointer
        newCST.deleteNode(newBranchBool);
        return false;
      }
    }
    else //while
    {
      expecting.push_back("while");
      newCST.curNode = newCST.curNode->parent; //kick back pointer
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
      newCST.addChild(newBranchBool, true);
      if(BooleanExpr())
      {
        Token* newBranchBlock = new Token("Block");
        newCST.addChild(newBranchBlock, true);
        if(Block())
        {
          expecting.clear();
          newCST.curNode = newBranchBlock->parent; //kick back pointer
          return true;
        }
        else //Block()
        {
          expecting.push_back("Block()");
          newCST.curNode = newBranchBlock->parent; //kick back pointer
          newCST.deleteNode(newBranchBlock);
          return false;
        }
      }
      else //BooleanExpr()
      {
        expecting.push_back("BooleanExpr()");
        newCST.curNode = newBranchBool->parent; //kick back pointer
        newCST.deleteNode(newBranchBool);
        return false;
      }
    }
    else //if
    {
      expecting.push_back("if");
      newCST.curNode = newCST.curNode->parent;
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
    newCST.addChild(newBranch, true);
    if(IntExpr())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //IntExpr()
    {
      expecting.push_back("IntExpr()");
      newCST.curNode = newBranch->parent;
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::Expr2() //StringExpr()
  {
    Token* newBranch = new Token("StringExpr");
    newCST.addChild(newBranch, true);
    if(StringExpr())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //StringExpr()
    {
      expecting.push_back("StringExpr()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::Expr3() //BooleanExpr()
  {
    Token* newBranch = new Token("BooleanExpr");
    newCST.addChild(newBranch, true);
    if(BooleanExpr())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //BooleanExpr()
    {
      expecting.push_back("BooleanExpr()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::Expr4() //Id()
  {
    Token* newBranch = new Token("Id");
    newCST.addChild(newBranch, true);
    if(Id())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else//Id()
    {
      expecting.push_back("Id()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
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
    newCST.addChild(newBranchDigit, true);
    if(digit())
    {
      if(term("intop"))
      {
        Token* newBranchExpr = new Token("Expr");
        newCST.addChild(newBranchExpr, true);
         if(Expr())
         {
           expecting.clear();
           newCST.curNode = newBranchExpr->parent; //kick back pointer
           return true;
         }
         else //Expr()
         {
           expecting.push_back("Expr()");
           newCST.curNode = newBranchExpr->parent; //kick back pointer
           newCST.deleteNode(newBranchExpr);
           return false;
         }
      }
      else //intop
      {
        expecting.push_back("intop");
        newCST.curNode = newCST.curNode->parent;
        return false;
      }
    }
    else //digit()
    {
      expecting.push_back("Digit()");
      newCST.curNode = newBranchDigit->parent;
      newCST.deleteNode(newBranchDigit);
      return false;
    }
  }
  bool Parser::IntExpr2() //digit()
  {
    Token* newBranch = new Token("Digit");
    newCST.addChild(newBranch, true);
    if(digit())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //digit()
    {
      expecting.push_back("Digit()");
      newCST.curNode = newBranch->parent;
      newCST.deleteNode(newBranch);
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
      newCST.addChild(newBranch, true);
      if(CharList())
      {
        if(term("rightQuote"))
        {
          expecting.clear();
          newCST.curNode = newBranch->parent; //kick back pointer
          return true;
        }
        else //rightQuote
        {
          expecting.push_back("\"");
          newCST.curNode = newCST.curNode->parent;
          return false;
        }
      }
      else //CharList()
      {
        expecting.push_back("CharList()");
        newCST.curNode = newBranch->parent;
        newCST.deleteNode(newBranch);
        return false;
      }
    }
    else //leftQuote
    {
      expecting.push_back("\"");
      newCST.curNode = newCST.curNode->parent;
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
      newCST.addChild(newBranchExpr1, true);
      if(Expr())
      {
        Token* newBranchBool = new Token("BoolOp");
        newCST.addChild(newBranchBool, true);
        if (boolop())
        {
          Token* newBranchExpr2 = new Token("Expr");
          newCST.addChild(newBranchExpr2, true);
          if(Expr())
          {
            if(term("rightParen"))
            {
              expecting.clear();
              newCST.curNode = newBranchExpr2->parent; //kick back pointer
              return true;
            }
            else //rightParen
            {
              expecting.push_back(")");
              newCST.curNode = newCST.curNode->parent;
              return false;
            }
          }
          else //Expr() #2
          {
            expecting.push_back("Expr()");
            newCST.curNode = newBranchExpr2->parent;
            newCST.deleteNode(newBranchExpr2);
            return false;
          }
        }
        else //boolop()
        {
          expecting.push_back("Boolop()");
          newCST.curNode = newBranchBool->parent;
          newCST.deleteNode(newBranchBool);
          return false;
        }
      }
      else //Expr()
      {
        expecting.push_back("Expr()");
        newCST.curNode = newBranchExpr1->parent;
        newCST.deleteNode(newBranchExpr1);
        return false;
      }
    }
    else //leftParen
    {
      expecting.push_back("(");
      newCST.curNode = newCST.curNode->parent;
      return false;
    }
  }
  bool Parser::BooleanExpr2() //boolval()
  {
    Token* newBranch = new Token("BoolVal");
    newCST.addChild(newBranch, true);
    if(boolval())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //boolval()
    {
      expecting.push_back("Boolval()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
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
    newCST.addChild(newBranch, true);
    if(Char())
    {
      expecting.clear();
      newCST.curNode = newBranch->parent; //kick back pointer
      return true;
    }
    else //Char()
    {
      expecting.push_back("Char()");
      newCST.curNode = newBranch->parent; //kick back pointer
      newCST.deleteNode(newBranch);
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
    newCST.addChild(newBranchChar, true);
    if(Char())
    {
      Token* newBranchCharL = new Token("CharList");
      newCST.addChild(newBranchCharL, true);
      if(CharList())
      {
        expecting.clear();
        newCST.curNode = newBranchCharL->parent; //kick back pointer
        return true;
      }
      else //CharList()
      {
        expecting.push_back("CharList()");
        newCST.curNode = newBranchCharL->parent; //kick back the pointer
        newCST.deleteNode(newBranchCharL);
        return false;
      }
    }
    else //Char()
    {
      expecting.push_back("Char()");
      newCST.curNode = newBranchChar->parent; //kick back pointer
      newCST.deleteNode(newBranchChar);
      return false;
    }
  }
  bool Parser::CharList2() //space() CharList()
  {
    Token* newBranchS = new Token("Space");
    newCST.addChild(newBranchS, true);
    if(space())
    {
      Token* newBranchCharL = new Token("CharList");
      newCST.addChild(newBranchCharL, true);
      if(CharList())
      {
        expecting.clear();
        newCST.curNode = newBranchCharL->parent; //kick back pointer
        return true;
      }
      else //CharList()
      {
        expecting.push_back("CharList()");
        newCST.curNode = newBranchCharL->parent;
        newCST.deleteNode(newBranchCharL);
        return false;
      }
    }
    else //space()
    {
      expecting.push_back("space()");
      newCST.curNode = newBranchS->parent;
      newCST.deleteNode(newBranchS);
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