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
    newCST.returnToRoot();  //go back to the root
    newCST.calcDepth = newCST.curNode; //set calc depth node
    newCST.dfio(newCST.curNode);
  }
  else
  {
    cout << "No segmentation faults, but the parse wasn't successful" << endl;
    for(auto i = 0; i < Parser::expecting.size(); i++)
    {
      cout << expecting[i] << endl;
    }
  }



}

  void Parser::kick()
  {
    cout << "before kick: " << newCST.curNode->getType() << endl;
    if(newCST.curNode->parent) //avoid null ptr
    {
      newCST.curNode = newCST.curNode->parent;
    }
    cout << "after kick: " << newCST.curNode->getType() << endl;
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
      cout << "hey we matched: " << tt << endl;
      return true;
    }
    else //no match
    {
      cout << "hey we failed: " << tt << endl;
      cout << "this is the type we couldn't match:  \"" << newCST.curNode->getType() << "\"" << endl;
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
      newCST.deleteNode(newBranch);
      return false;
    }
  }

  bool Parser::Program()
  {
    int save = Parser::i;
    if (Parser::i = save, Program1())
    {
      kick(); //kick back to start
      return true;
    } else
    {
      {
        kick(); //kick back to start
      }
      return false;
    }
  }

  //BLOCK=================================================================

  bool Parser::Block1()  //leftBrace, StatementList(), rightBrace
  {
    if(term("leftBrace"))
    {
      Token* newBranch = new Token("StatementList1"); //add token preemtively
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
    int save = Parser::i;
    if (Parser::i = save, Block1())
    {
      kick(); //kick back to start
      return true;
    }
    else
    {
      kick(); //kick back to start
      return false;
    }
  }

  //STATEMENTLIST=========================================================

  bool Parser::StatementList1() //Statement() StatementList()
  {
    Token* newBranchS = new Token("Statement");
    newCST.addChild(newBranchS, true);
    if (Statement())
    {
      cout << "Where we at dawg: " << newCST.curNode->getType() << endl;
      //kick();
      cout << "Where we at now dawg: " << newCST.curNode->getType() << endl;
      Token* newBranchSL = new Token ("StatementList2");
      newCST.addChild(newBranchSL,true);
      if (StatementList())
      {
        expecting.clear();
        return true;
      }
      else //StatementList()
      {
        expecting.push_back("StatementList()");
        newCST.deleteNode(newBranchSL);
        return false;
      }
    }
    else //Statement()
    {
      expecting.push_back("Statement()");
      newCST.deleteNode(newBranchS);
      return false;
    }
  }
  bool Parser::StatementList2() //epsilon
  {
    expecting.clear();
    cout << "epsilon statementlist: " << endl;
    return true;
  }

  bool Parser::StatementList()
  {
    cout << "hey I'm at the statementlist: " << newCST.curNode->getType() << endl;
    int save = Parser::i;
    if(Parser::i = save, StatementList1())
    {
      kick(); //kick back
      return true;
    }
    else if (Parser::i = save, StatementList2())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //STATEMENT=============================================================

  bool Parser::Statement1() //PrintStatement()
  {
    Token* newBranch = new Token("PrintStatement");
    newCST.addChild(newBranch, true);
    if(PrintStatement())
    {
      expecting.clear();
      return true;
    }
    else //PrintStatement()
    {
      expecting.push_back("PrintStatement()");
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
      return true;
    }
    else //AssignmentStatement()
    {
      expecting.push_back("AssignmentStatement()");
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
      return true;
    }
    else //VarDecl()
    {
      expecting.push_back("VarDecl()");
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
      return true;
    }
    else //WhileStatement()
    {
      expecting.push_back("WhileStatement()");
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
      return true;
    }
    else //IfStatement()
    {
      expecting.push_back("IfStatement()");
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
      return true;
    }
    else //Block()
    {
      expecting.push_back("BlockStatement()");
      newCST.deleteNode(newBranch);
      return false;
    }
  }

  bool Parser::Statement()
  {
    int save = Parser::i;
    if(Parser::i = save, Statement1())
    {
      kick(); //kick back
      return true;
    }
    else if (Parser::i = save, Statement2())
    {
      kick(); //kick back
      return true;
    }
    else if (Parser::i = save, Statement3())
    {
      kick(); //kick back
      return true;
    }
    else if (Parser::i = save, Statement4())
    {
      kick(); //kick back
      return true;
    }
    else if (Parser::i = save, Statement5())
    {
      kick(); //kick back
      return true;
    }
    else if (Parser::i = save, Statement6())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
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
            return true;
          }
          else //rightParen
          {
            expecting.push_back("(");
            return false;
          }
        }
        else  //Expr()
        {
          expecting.push_back("Expr()");
          newCST.deleteNode(newBranch);
          return false;
        }
      }
      else //leftParen
      {
        expecting.push_back("(");
        return false;
      }
    }
    else //print
    {
      expecting.push_back("print");
      return false;
    }
  }

  bool Parser::PrintStatement()
  {
    int save = Parser::i;
    if (Parser::i = save, PrintStatement1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
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
          return true;
        }
        else //Expr()
        {
          expecting.push_back("Expr()");
          newCST.deleteNode(newBranchE);
          return false;
        }
      }
      else //=
      {
        expecting.push_back("=");
        return false;
      }
    }
    else //Id()
    {
      expecting.push_back("Id()");
      newCST.deleteNode(newBranchI);
      return false;
    }
    
  }

  bool Parser::AssignmentStatement()
  {
    int save = Parser::i;
    if (Parser::i = save, AssignmentStatement1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //VARDECL===============================================================

  bool Parser::VarDecl1()
  {
    Token* newBranchT = new Token("type");
    newCST.addChild(newBranchT, true);
    if(type())
    {
      Token* newBranchI = new Token("Id");
      newCST.addChild(newBranchI, true);
      if(Id())
      {
        expecting.clear();
        return true;
      }
      else //Id()
      {
        expecting.push_back("Id()");
        newCST.deleteNode(newBranchI);
        return false;
      }
    }
    else //type()
    {
      expecting.push_back("Type()");
      newCST.deleteNode(newBranchT);
      return false;
    }
  }
  bool Parser::VarDecl()
  {
    int save = Parser::i;
    if(Parser::i = save, VarDecl1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
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
          return true;
        }
        else //Block()
        {
          expecting.push_back("Block()");
          newCST.deleteNode(newBranchBlock);
          return false;
        }
      }
      else //BooleanExpr()
      {
        expecting.push_back("BooleanExpr()");
        newCST.deleteNode(newBranchBool);
        return false;
      }
    }
    else //while
    {
      expecting.push_back("while");
      return false;
    }
  }

  bool Parser::WhileStatement()
  {
    int save = Parser::i;
    if(Parser::i = save, WhileStatement1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
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
          return true;
        }
        else //Block()
        {
          expecting.push_back("Block()");
          newCST.deleteNode(newBranchBlock);
          return false;
        }
      }
      else //BooleanExpr()
      {
        expecting.push_back("BooleanExpr()");
        newCST.deleteNode(newBranchBool);
        return false;
      }
    }
    else //if
    {
      expecting.push_back("if");
      return false;
    }
  }
  bool Parser::IfStatement()
  {
    int save = Parser::i;
    if( Parser::i = save, IfStatement1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //EXPR==================================================================

  bool Parser::Expr1()  //IntExpr()
  {
    Token* newBranch = new Token("IntExpr");
    newCST.addChild(newBranch, true);
    if(IntExpr())
    {
      expecting.clear();
      return true;
    }
    else //IntExpr()
    {
      expecting.push_back("IntExpr()");
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
      return true;
    }
    else //StringExpr()
    {
      expecting.push_back("StringExpr()");
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
      return true;
    }
    else //BooleanExpr()
    {
      expecting.push_back("BooleanExpr()");
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
      return true;
    }
    else//Id()
    {
      expecting.push_back("Id()");
      newCST.deleteNode(newBranch);
      return false;
    }
  }

  bool Parser::Expr()
  {
    int save = Parser::i;
    if( Parser::i = save, Expr1())
    {
      kick(); //kick back
      return true;
    }
    else if( Parser::i = save, Expr2())
    {
      kick(); //kick back
      return true;
    }
    else if ( Parser::i = save, Expr3())
    {
      kick(); //kick back
      return true;
    }
    else if( Parser::i = save, Expr4())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //INTEXPR===============================================================

  bool Parser::IntExpr1() //digit() intop Expr()
  {
    Token* newBranchDigit = new Token("digit");
    newCST.addChild(newBranchDigit, true);
    if(digit())
    {
      Token* newBranchIntOp = new Token("IntOp");
      newCST.addChild(newBranchIntOp, true);
      if(intop())
      {
        Token* newBranchExpr = new Token("Expr");
        newCST.addChild(newBranchExpr, true);
         if(Expr())
         {
           expecting.clear();
           return true;
         }
         else //Expr()
         {
           expecting.push_back("Expr()");
           newCST.deleteNode(newBranchExpr);
           return false;
         }
      }
      else //intop
      {
        expecting.push_back("Intop()");
        newCST.deleteNode(newBranchIntOp);
        return false;
      }
    }
    else //digit()
    {
      expecting.push_back("Digit()");
      newCST.deleteNode(newBranchDigit);
      return false;
    }
  }
  bool Parser::IntExpr2() //digit()
  {
    Token* newBranch = new Token("digit");
    newCST.addChild(newBranch, true);
    if(digit())
    {
      expecting.clear();
      return true;
    }
    else //digit()
    {
      expecting.push_back("Digit()");
      newCST.deleteNode(newBranch);
      return false;
    }
  }

  bool Parser::IntExpr()
  {
    int save = Parser::i;
    if( Parser::i = save, IntExpr1())
    {
      kick(); //kick back
      return true;
    }
    else if( Parser::i = save, IntExpr2())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
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
          return true;
        }
        else //rightQuote
        {
          expecting.push_back("\"");
          return false;
        }
      }
      else //CharList()
      {
        expecting.push_back("CharList()");
        newCST.deleteNode(newBranch);
        return false;
      }
    }
    else //leftQuote
    {
      expecting.push_back("\"");
      return false;
    }
  }
  bool Parser::StringExpr()
  {
    int save = Parser::i;
    if( Parser::i = save, StringExpr1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
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
              return true;
            }
            else //rightParen
            {
              expecting.push_back(")");
              return false;
            }
          }
          else //Expr() #2
          {
            expecting.push_back("Expr()");
            newCST.deleteNode(newBranchExpr2);
            return false;
          }
        }
        else //boolop()
        {
          expecting.push_back("Boolop()");
          newCST.deleteNode(newBranchBool);
          return false;
        }
      }
      else //Expr()
      {
        expecting.push_back("Expr()");
        newCST.deleteNode(newBranchExpr1);
        return false;
      }
    }
    else //leftParen
    {
      expecting.push_back("(");
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
      return true;
    }
    else //boolval()
    {
      expecting.push_back("Boolval()");
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::BooleanExpr()
  {
    int save = Parser::i;
    if( Parser::i = save, BooleanExpr1())
    {
      kick(); //kick back
      return true;
    }
    else if( Parser::i = save, BooleanExpr2())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //ID====================================================================

  bool Parser::Id1() //Char()
  {
    Token* newBranch = new Token("char");
    newCST.addChild(newBranch, true);
    if(Char())
    {
      expecting.clear();
      return true;
    }
    else //Char()
    {
      expecting.push_back("Char()");
      newCST.deleteNode(newBranch);
      return false;
    }
  }
  bool Parser::Id()
  {
    int save = Parser::i; if ( Parser::i = save, Id1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //CHARLIST==============================================================

  bool Parser::CharList1() //Char() CharList()
  {
    Token* newBranchChar = new Token("char");
    newCST.addChild(newBranchChar, true);
    if(Char())
    {
      Token* newBranchCharL = new Token("CharList");
      newCST.addChild(newBranchCharL, true);
      if(CharList())
      {
        expecting.clear();
        return true;
      }
      else //CharList()
      {
        expecting.push_back("CharList()");
        newCST.deleteNode(newBranchCharL);
        return false;
      }
    }
    else //Char()
    {
      expecting.push_back("Char()");
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
        return true;
      }
      else //CharList()
      {
        expecting.push_back("CharList()");
        newCST.deleteNode(newBranchCharL);
        return false;
      }
    }
    else //space()
    {
      expecting.push_back("space()");
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
    int save = Parser::i;
    if ( Parser::i = save, CharList1())
    {
      kick(); //kick back
      return true;
    }
    else if ( Parser::i = save, CharList2())
    {
      kick(); //kick back
      return true;
    }
    else if( Parser::i = save, CharList3())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //TYPE==================================================================

  bool Parser::type1() //type
  {
    if(term("type"))
    {
      expecting.clear();
      return true;
    }
    else //type
    {
      expecting.push_back("type");
      return false;
    }
  }
  bool Parser::type()
  {
    int save = Parser::i;
    if ( Parser::i = save, type1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //CHAR==================================================================

  bool Parser::Char1()
  {
    if(term("char"))
    {
      expecting.clear();
      return true;
    }
    else //Char
    {
      expecting.push_back("Char()");
      return false;
    }
  }
  bool Parser::Char()
  {
    int save = Parser::i;
    if ( Parser::i = save, Char1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //SPACE=================================================================

  bool Parser::space1()
  {
    if(term(" "))
    {
      expecting.clear();
      return true;
    }
    else //space
    {
      expecting.push_back("space_char");
      return false;
    }
  }
  bool Parser::space()
  {
    int save = Parser::i;
    if ( Parser::i = save, space1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //DIGIT=================================================================

  bool Parser::digit1()
  {
    if(term("digit"))
    {
      expecting.clear();
      return true;
    }
    else //digit
    {
      expecting.push_back("digit");
      return false;
    }
  }
  bool Parser::digit()
  {
    int save = Parser::i;
    if( Parser::i = save, digit1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //BOOLOP================================================================

  bool Parser::boolop1()
  {
    if(term("boolOp"))
    {
      expecting.clear();
      return true;
    }
    else //boolop
    {
      expecting.push_back("boolop");
      return false;
    }
  }
  bool Parser::boolop()
  {
    int save = Parser::i;
    if( Parser::i = save, boolop1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //BOOLVAL===============================================================

  bool Parser::boolval1()
  {
    if(term("boolVal"))
    {
      expecting.clear();
      return true;
    }
    else //boolval
    {
      expecting.push_back("boolval");
      return false;
    }
  }
  bool Parser::boolval()
  {
    int save = Parser::i;
    if( Parser::i = save, boolval1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }

  //INTOP=================================================================

  bool Parser::intop1() {
    if(term("intOp"))
    {
      expecting.clear();
      return true;
    }
    else //intop
    {
      expecting.push_back("intop");
      return false;
    }
  }
  bool Parser::intop()
  {
    int save = Parser::i;
    if( Parser::i = save, intop1())
    {
      kick(); //kick back
      return true;
    }
    else
    {
      kick(); //kick back
      return false;
    }
  }