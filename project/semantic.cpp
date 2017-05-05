#include <iostream>
#include <vector>
#include <string>

#include "semantic.h"
#include "symbolTable.h"
#include "stEntry.h"
#include "error.h"
#include "genSymbolTable.h"

using namespace std;

Semantic::Semantic(vector<Token> stream, bool v, unsigned int start)  //v is for verbose
  : verbose(v), i(start), s(start)
{

  //remember, if a process fails, we need to delete all of the children
  Semantic::tokens = stream;

  if(Program())
  {
    //avoid segmentation fault by exceeding the maximum bound of stream
    if(i >= stream.size())
    {
      //yes I know I'm casting long to int
      i = static_cast<int>(stream.size() - 1);
    }

    cout << "Successful Semantic Analysis!" << endl;
    cout << "Printing SA to output.html" << endl;

    //put here just in case
    newAST.rootToken = newAST.returnToRoot();  //go back to the root

    //print out the AST in the command line
    newAST.rootToken = newAST.returnToRoot();  //go back to the root
    newAST.calcDepth = newAST.curNode; //set calc depth node
    newAST.dfio(newAST.curNode, verbose);

    //symbol table generator
    newGen = new GenSymbolTable(newAST.rootToken, verbose);

    //push back all ST tables
    for(vector<string>::size_type l = 0; l < newGen->symbolTableOutput.size(); l++)
    {
      genSTOutput.push_back(newGen->symbolTableOutput[l]);
    }
    //recursively call Semantic if leftover tokens
    if(i != (stream.size() -1))
    {
      //push break between new tables
      genSTOutput.push_back("</br></br></br></br></br></br>");
      Semantic recursiveSemantic(stream, verbose, i);
      cout << tokens[i].getType() << " data" << tokens[i].getData() << endl;
      *recursiveSemantic.newGen = GenSymbolTable(&tokens[i], verbose);
      //push back ast to original ast
      for(vector<string>::size_type j = 0; j < recursiveSemantic.newAST.tree.size(); j++)
      {
        newAST.tree.push_back(recursiveSemantic.newAST.tree[j]);
      }
      //push back the other symbol tables to the original semantic object
      for(vector<string>::size_type m = 0; m < recursiveSemantic.newGen->symbolTableOutput.size(); m++)
      {
        genSTOutput.push_back(recursiveSemantic.newGen->symbolTableOutput[m]);
      }
    }
  }
  else //error
  {
    //TODO: deal with error
  }

}

Semantic::~Semantic()
{
  //delete newGen;
}

//kick nodes in the AST
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


bool Semantic::term(string tt) //terminal leaf creation
{
  if (Semantic::tokens[(Semantic::i)].getType() == tt)
  {
    //time to discriminate and keep what tokens we want
    if  ((tt != "leftBrace") //don't want these
         && (tt != "rightBrace")
         && (tt != "leftParen")
         && (tt != "rightParen")
         && (tt != "leftQuote")
         && (tt != "rightQuote")
         && (tt != "print")
         && (tt != "while")
         && (tt != "boolOp")
         && (tt != "if")
         && (tt != "assign")
         && (tt != "intOp")
      )
    {
      //put token we want on the heap
      Token* newTerminal = new Token(Semantic::tokens[Semantic::i]);
      //change charList to string
      if(newTerminal->getType() == "charList")
      {
        newTerminal->setType("string");
      }
      //create leaf
      newAST.addChild(newTerminal, false, verbose);

    }

    if(verbose)
    {
      cout << "---hey we matched: " << tt << endl;
    }
    ++i;  //increment i
    return true;
  }


  else //no match
  {
    //handle empty charlists
    if(tt == "charList")
    {
      if(Semantic::tokens[(Semantic::i - 1)].getType() == "leftQuote"
        &&Semantic::tokens[(Semantic::i)].getType() == "rightQuote")
      {
        int line = Semantic::tokens[Semantic::i].getLine();
        int pos = Semantic::tokens[Semantic::i].getPos();
        Token* emptyCharList = new Token("string","", line, pos);
        newAST.addChild(emptyCharList, false, verbose);
        //don't increment i
        return true;
      }
    }
    if (verbose)
    {
      cout << "><><?hey we failed: " << tt << endl;
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
  unsigned int save = Semantic::i;
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
  unsigned int save = Semantic::i;
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
  unsigned int save = Semantic::i;
  if(Semantic::i = save, StatementList1())
  {
    //kick()
    return true;
  }
  else if (Semantic::i = save, StatementList2())
  {
    //kick()
    return true;
  }
  else
  {
    //kick()
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
  unsigned int save = Semantic::i;
  if(Semantic::i = save, Statement1())
  {

    return true;
  }
  else if (Semantic::i = save, Statement2())
  {

    return true;
  }
  else if (Semantic::i = save, Statement3())
  {

    return true;
  }
  else if (Semantic::i = save, Statement4())
  {

    return true;
  }
  else if (Semantic::i = save, Statement5())
  {

    return true;
  }
  else if (Semantic::i = save, Statement6())
  {

    return true;
  }
  else
  {

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
  unsigned int save = Semantic::i;
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
  unsigned int save = Semantic::i;
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
  unsigned int save = Semantic::i;
  if(Semantic::i = save, VarDecl1())
  {
    //vardecl succeeded
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
  unsigned int save = Semantic::i;
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
  unsigned int save = Semantic::i;
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
  Token* newBranch = new Token("+");
  newAST.addChild(newBranch, true, verbose);
  if(IntExpr())
  {
    return true;
  }
  else //IntExpr()
  {
    newAST.deleteNode(newBranch);
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
  if(BooleanExpr())
  {
    return true;
  }
  else //BooleanExpr()
  {
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
  unsigned int save = Semantic::i;
  if( Semantic::i = save, Expr1())
  {
    
    return true;
  }
  else if( Semantic::i = save, Expr2())
  {
    
    return true;
  }
  else if ( Semantic::i = save, Expr3())
  {

    return true;
  }
  else if( Semantic::i = save, Expr4())
  {
    
    return true;
  }
  else
  {
    
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
      ///get rid of the double
    else //intOp
    {
      //curNode at intExpr, but intExpr->num is still there
      delete newAST.curNode->children.back(); //delete num
      newAST.curNode->children.clear(); //clear all children references of intExpr
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
    /*
     * Hey future me! This might look confusing at first. Why?!
     * Basically, an unwanted IntExpr prints during IntExpr2
     * because an intExpr can turn into an intVal. As a result
     * we would get an extra + over every digit and we don't
     * want that
     *
     * This code below gets rid of the extra intExpr by messing
     * with pointers. Good luck!
     */


    Token* copyCur = newAST.curNode;
    Token* copyChild = newAST.curNode->children.back();
    newAST.curNode = newAST.curNode->parent; //move to parent

    copyChild->parent = copyCur->parent; //set parent of copied child
    newAST.curNode->children.back() = copyChild; //get rid of reference to intExpr
    delete copyCur; //free up copyCur

    return true;

  }
  else //digit()
  {
    return false;
  }
}

bool Semantic::IntExpr()
{
  unsigned int save = Semantic::i;
  if( Semantic::i = save, IntExpr1())
  {
    kick();
    return true;
  }
  else if( Semantic::i = save, IntExpr2())
  {
    //kick();
    return true;
  }
  else
  {
    kick();
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
  unsigned int save = Semantic::i;
  if( Semantic::i = save, StringExpr1())
  {
    return true;
  }
  else
  {
    return false;
  }
}

//BOOLEANEXPRESSION===========================================================

bool Semantic::BooleanExpr1() //leftParen Expr() boolop() Expr() rightParen
{
  if(term("leftParen"))
  {

    Token* newComparison = new Token("Comp");
    newAST.addChild(newComparison,true, verbose);
    if(Expr())
    {
      pair<bool,string> boolOpResults = boolop();
      if (boolOpResults.first) //boolop succeeded
      {
        //grab value of it and change comp operator
        newComparison->setType(boolOpResults.second);
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
  unsigned int save = Semantic::i;
  if( Semantic::i = save, BooleanExpr1())
  {

    kick();
    return true;
  }
  else if( Semantic::i = save, BooleanExpr2())
  {
    return true;
  }
  else
  {
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
  unsigned int save = Semantic::i; if ( Semantic::i = save, Id1())
  {
    
    return true;
  }
  else
  {
    
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
  unsigned int save = Semantic::i;
  if ( Semantic::i = save, CharList1())
  {
    
    return true;
  }
  else if ( Semantic::i = save, CharList2())
  {
    return true;
  }
  else
  {
    
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
  unsigned int save = Semantic::i;
  if ( Semantic::i = save, type1())
  {
    
    return true;
  }
  else
  {
    
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
  unsigned int save = Semantic::i;
  if ( Semantic::i = save, Char1())
  {
    return true;
  }
  else
  {
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
  unsigned int save = Semantic::i;
  if ( Semantic::i = save, space1())
  {
    
    return true;
  }
  else
  {
    
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
  unsigned int save = Semantic::i;
  if( Semantic::i = save, digit1())
  {

    return true;
  }
  else
  {

    return false;
  }
}

//BOOLOP================================================================

pair<bool,string> Semantic::boolop() {
  unsigned int save = Semantic::i;

  if (Semantic::i = save, term("boolOp"))
  {
    Semantic::i = save;
    //grab the token we are comparing
    string value = Semantic::tokens[Semantic::i].getData();
    Semantic::i++;
    return make_pair(true, value);
  } else //boolop
  {
    return make_pair(false, "");
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
  unsigned int save = Semantic::i;
  if( Semantic::i = save, boolval1())
  {
    return true;
  }
  else
  {
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
  unsigned int save = Semantic::i;
  if( Semantic::i = save, intop1())
  {
    return true;
  }
  else
  {
    return false;
  }
}