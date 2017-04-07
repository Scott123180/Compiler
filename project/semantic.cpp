#include <iostream>
#include <vector>
#include <string>

#include "AST.h"
#include "semantic.h"

Semantic::Semantic(vector<Token> stream, bool v, unsigned int start)  //v is for verbose
  : verbose(v), i(start)
{

  //remember, if a process fails, we need to delete all of the children
  Semantic::tokens = stream;

  if(Program())
  {
    cout << "Successful Semantic Analysis!" << endl;
    cout << "Printing SA to output.html" << endl;


    //print out the AST in the command line
    newAST.returnToRoot();  //go back to the root
    newAST.calcDepth = newAST.curNode; //set calc depth node
    newAST.dfio(newAST.curNode, verbose);

    //recursively call Semantic if leftover tokens
    if(i != stream.size())
    {
      Semantic recursiveSemantic(stream, verbose, i);
      for(vector<string>::size_type j = 0; j < recursiveSemantic.newAST.tree.size(); j++)
      {
        newAST.tree.push_back(recursiveSemantic.newAST.tree[j]);
      }
    }

  }
  else //error
  {
    //TODO: deal with error
  }

}

//copy pasta
void Semantic::kick()
{
  if (verbose)
  {
    cout << "before kick: " << newAST.curNode->getType() << endl;
  }
  if(newAST.curNode->parent) //avoid null ptr
  {
    newAST.curNode = newAST.curNode->parent;
  }
  if(verbose)
  {
    cout << "after kick: " << newAST.curNode->getType() << endl;
  }
}
/*

 bool term(TOKEN tok) {return *next++ == tok; }
   returns true if the token we passed in matches the input or no it doesn't
 */

bool Semantic::term(string tt) //terminal leaf creation
{
  if (Semantic::tokens[(Semantic::i)].getType() == tt)
  {
    ++i; //increment i
    if(verbose)
    {
      cout << "hey we matched: " << tt << endl;
    }
    return true;
  }
  else //no match
  {
    if (verbose)
    {
      cout << "hey we failed: " << tt << endl;
      cout << "this is the type we couldn't match:  \"" << newAST.curNode->getType() << "\"" << endl;
    }

    ++i; //increment i regardless
    return false;
  }
}


//PROGRAM===============================================================
//already added a program token as our root node
bool Semantic::Program1()
{
  //create block branch
  Token* newBranch = new Token("Block");
  newAST.addChild(newBranch, true, verbose);
  if(Block())
  {
    if(term("EOP"))
    {
      return true;
    }
    else
    {
      return false;
    } //NOT EOP
  }
  else //Block()
  {
    newAST.deleteNode(newBranch);
    return false;
  }
}

bool Semantic::Program()
{
  int save = Semantic::i;
  if (Semantic::i = save, Program1())
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

//BLOCK=================================================================

bool Semantic::Block1()  //leftBrace, StatementList(), rightBrace
{
  if(term("leftBrace"))
  {
    if(StatementList())
    {
      if(term("rightBrace"))
      {
        return true;
      }
    }
    else //fail StatementList()
    {
      return false;
    }
  }
  else //fail leftBrace
  {
    return false;
  }
}

bool Semantic::Block()
{
  int save = Semantic::i;
  if (Semantic::i = save, Block1())
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

bool Semantic::StatementList1() //Statement() StatementList()
{
  if (Statement())
  {
    if (StatementList())
    {
      
      return true;
    }
    else //StatementList()
    {
      return false;
    }
  }
  else //Statement()
  {
    return false;
  }
}
bool Semantic::StatementList2() //epsilon
{
  return true;
}

bool Semantic::StatementList()
{
  int save = Semantic::i;
  if(Semantic::i = save, StatementList1())
  {
    kick(); //kick back
    return true;
  }
  else if (Semantic::i = save, StatementList2())
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

bool Semantic::Statement1() //PrintStatement()
{
  Token* newBranch = new Token("PrintStatement");
  newAST.addChild(newBranch, true, verbose);
  if(PrintStatement())
  {
    
    return true;
  }
  else //PrintStatement()
  {
    newAST.deleteNode(newBranch);
    return false;
  }
}
bool Semantic::Statement2() //AssignStatement()
{
  Token* newBranch = new Token("AssignStatement");
  newAST.addChild(newBranch, true, verbose);
  if(AssignmentStatement())
  {
    
    return true;
  }
  else //AssignmentStatement()
  {
    newAST.deleteNode(newBranch);
    return false;
  }
}
bool Semantic::Statement3() //VarDecl()
{
  Token* newBranch = new Token("VarDecl");
  newAST.addChild(newBranch, true, verbose);
  if(VarDecl())
  {
    return true;
  }
  else //VarDecl()
  {
    newAST.deleteNode(newBranch);
    return false;
  }
}
bool Semantic::Statement4() //WhileStatement()
{
  Token* newBranch = new Token("WhileStatement");
  newAST.addChild(newBranch, true, verbose);
  if(WhileStatement())
  {
    return true;
  }
  else //WhileStatement()
  {
    newAST.deleteNode(newBranch);
    return false;
  }
}
bool Semantic::Statement5() //IfStatement()
{
  Token* newBranch = new Token("IfStatement");
  newAST.addChild(newBranch, true, verbose);
  if(IfStatement())
  {
    
    return true;
  }
  else //IfStatement()
  {
    newAST.deleteNode(newBranch);
    return false;
  }
}
bool Semantic::Statement6() //Block()
{
  Token* newBranch = new Token("Block");
  newAST.addChild(newBranch, true, verbose);
  if(Block())
  {
    return true;
  }
  else //Block()
  {
    newAST.deleteNode(newBranch);
    return false;
  }
}

bool Semantic::Statement()
{
  int save = Semantic::i;
  if(Semantic::i = save, Statement1())
  {
    kick(); //kick back
    return true;
  }
  else if (Semantic::i = save, Statement2())
  {
    kick(); //kick back
    return true;
  }
  else if (Semantic::i = save, Statement3())
  {
    kick(); //kick back
    return true;
  }
  else if (Semantic::i = save, Statement4())
  {
    kick(); //kick back
    return true;
  }
  else if (Semantic::i = save, Statement5())
  {
    kick(); //kick back
    return true;
  }
  else if (Semantic::i = save, Statement6())
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

bool Semantic::PrintStatement1() //print leftParen Expr() rightParen
{
  if (term("print"))
  {
    if (term("leftParen"))
    {
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

bool Semantic::PrintStatement()
{
  int save = Semantic::i;
  if (Semantic::i = save, PrintStatement1())
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

bool Semantic::AssignmentStatement1()  //Id = Expr()
{
  if(Id())
  {
    if(term("assign"))
    {
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

bool Semantic::AssignmentStatement()
{
  int save = Semantic::i;
  if (Semantic::i = save, AssignmentStatement1())
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

bool Semantic::VarDecl1()
{
  if(type())
  {
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
bool Semantic::VarDecl()
{
  int save = Semantic::i;
  if(Semantic::i = save, VarDecl1())
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

bool Semantic::WhileStatement1()  //while BoolExpr() Block()
{
  if(term("while"))
  {
    Token* newBranchBool = new Token("BooleanExpr");
    newAST.addChild(newBranchBool, true, verbose);
    if(BooleanExpr())
    {
      Token* newBranchBlock = new Token("Block");
      newAST.addChild(newBranchBlock, true, verbose);
      if(Block())
      {
        
        return true;
      }
      else //Block()
      {
        newAST.deleteNode(newBranchBlock);
        return false;
      }
    }
    else //BooleanExpr()
    {
      newAST.deleteNode(newBranchBool);
      return false;
    }
  }
  else //while
  {
    return false;
  }
}

bool Semantic::WhileStatement()
{
  int save = Semantic::i;
  if(Semantic::i = save, WhileStatement1())
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

bool Semantic::IfStatement1() //if BooleanExpr() Block()
{
  if(term("if"))
  {
    Token* newBranchBool = new Token("BooleanExpr");
    newAST.addChild(newBranchBool, true, verbose);
    if(BooleanExpr())
    {
      Token* newBranchBlock = new Token("Block");
      newAST.addChild(newBranchBlock, true, verbose);
      if(Block())
      {
        
        return true;
      }
      else //Block()
      {
        newAST.deleteNode(newBranchBlock);
        return false;
      }
    }
    else //BooleanExpr()
    {
      newAST.deleteNode(newBranchBool);
      return false;
    }
  }
  else //if
  {
    return false;
  }
}
bool Semantic::IfStatement()
{
  int save = Semantic::i;
  if( Semantic::i = save, IfStatement1())
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

bool Semantic::Expr1()  //IntExpr()
{
  if(IntExpr())
  {
    return true;
  }
  else //IntExpr()
  {
    return false;
  }
}
bool Semantic::Expr2() //StringExpr()
{
  if(StringExpr())
  {
    return true;
  }
  else //StringExpr()
  {
    return false;
  }
}
bool Semantic::Expr3() //BooleanExpr()
{
  Token* newBranch = new Token("BooleanExpr");
  newAST.addChild(newBranch, true, verbose);
  if(BooleanExpr())
  {
    return true;
  }
  else //BooleanExpr()
  {
    newAST.deleteNode(newBranch);
    return false;
  }
}
bool Semantic::Expr4() //Id()
{
  if(Id())
  {
    
    return true;
  }
  else//Id()
  {
    return false;
  }
}

bool Semantic::Expr()
{
  int save = Semantic::i;
  if( Semantic::i = save, Expr1())
  {
    kick(); //kick back
    return true;
  }
  else if( Semantic::i = save, Expr2())
  {
    kick(); //kick back
    return true;
  }
  else if ( Semantic::i = save, Expr3())
  {
    kick(); //kick back
    return true;
  }
  else if( Semantic::i = save, Expr4())
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

bool Semantic::IntExpr1() //digit() intop Expr()
{
  if(digit())
  {
    if(intop())
    {
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
bool Semantic::IntExpr2() //digit()
{
  if(digit())
  {
    
    return true;
  }
  else //digit()
  {
    return false;
  }
}

bool Semantic::IntExpr()
{
  int save = Semantic::i;
  if( Semantic::i = save, IntExpr1())
  {
    kick(); //kick back
    return true;
  }
  else if( Semantic::i = save, IntExpr2())
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

bool Semantic::StringExpr1() //leftQuote CharList() rightQuote
{
  if(term("leftQuote"))
  {
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
bool Semantic::StringExpr()
{
  int save = Semantic::i;
  if( Semantic::i = save, StringExpr1())
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

bool Semantic::BooleanExpr1() //leftParen Expr() boolop() Expr() rightParen
{
  if(term("leftParen"))
  {
    if(Expr())
    {
      if (boolop())
      {
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
        else //Expr() #2
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
bool Semantic::BooleanExpr2() //boolval()
{
  if(boolval())
  {
    return true;
  }
  else //boolval()
  {
    return false;
  }
}
bool Semantic::BooleanExpr()
{
  int save = Semantic::i;
  if( Semantic::i = save, BooleanExpr1())
  {
    kick(); //kick back
    return true;
  }
  else if( Semantic::i = save, BooleanExpr2())
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

bool Semantic::Id1() //Char()
{
  if(Char())
  {
    return true;
  }
  else //Char()
  {
    return false;
  }
}
bool Semantic::Id()
{
  int save = Semantic::i; if ( Semantic::i = save, Id1())
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

bool Semantic::CharList1() //Char() CharList()
{
  if(term("charList"))
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool Semantic::CharList2() //epsilon
{
  return true;
}
bool Semantic::CharList()
{
  int save = Semantic::i;
  if ( Semantic::i = save, CharList1())
  {
    kick(); //kick back
    return true;
  }
  else if ( Semantic::i = save, CharList2())
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

bool Semantic::type1() //type
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
bool Semantic::type()
{
  int save = Semantic::i;
  if ( Semantic::i = save, type1())
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

bool Semantic::Char1()
{
  if(term("char"))
  {
    return true;
  }
  else //Char
  {
    return false;
  }
}
bool Semantic::Char()
{
  int save = Semantic::i;
  if ( Semantic::i = save, Char1())
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

bool Semantic::space1()
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
bool Semantic::space()
{
  int save = Semantic::i;
  if ( Semantic::i = save, space1())
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

bool Semantic::digit1()
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
bool Semantic::digit()
{
  int save = Semantic::i;
  if( Semantic::i = save, digit1())
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

bool Semantic::boolop1()
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
bool Semantic::boolop()
{
  int save = Semantic::i;
  if( Semantic::i = save, boolop1())
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

bool Semantic::boolval1()
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
bool Semantic::boolval()
{
  int save = Semantic::i;
  if( Semantic::i = save, boolval1())
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

bool Semantic::intop1() {
  if(term("intOp"))
  {
    return true;
  }
  else //intop
  {
    return false;
  }
}
bool Semantic::intop()
{
  int save = Semantic::i;
  if( Semantic::i = save, intop1())
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