#include <iostream>
#include <vector>
#include <string>

#include "semantic.h"
#include "symbolTable.h"
#include "stEntry.h"
#include "error.h"

Semantic::Semantic(vector<Token> stream, bool v, unsigned int start)  //v is for verbose
  : verbose(v), i(start)
{

  //remember, if a process fails, we need to delete all of the children
  Semantic::tokens = stream;

  //initialize symbol table; *this is reference to current semantic object
  SymbolTable* rootSymbolTable = new SymbolTable(nullptr, uniqueScope);
  curSymbolTable = rootSymbolTable;
  ++uniqueScope; //increment unique scope


  if(Program())
  {
    cout << "Successful Semantic Analysis!" << endl;
    cout << "Printing SA to output.html" << endl;


    //print out the AST in the command line
    newAST.returnToRoot();  //go back to the root
    newAST.calcDepth = newAST.curNode; //set calc depth node
    newAST.dfio(newAST.curNode, verbose);

    calcSymbolTableOutput(rootSymbolTable, false);

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

//kick nodes in the syntax tree
void Semantic::kickST()
{
  if (verbose)
  {
    cout << "===Symbol table - before kick: " << curSymbolTable->scope << endl;
  }
  if(curSymbolTable->parent) //avoid null ptr
  {
    curSymbolTable = curSymbolTable->parent;
  }
  if(verbose)
  {
    cout << "===Symbol table - after kick: " << curSymbolTable->scope << endl;
  }
}

//calculate the output for the symbol table
void Semantic::calcSymbolTableOutput(SymbolTable* a, bool verbose) //depth-first in order
{
  cout << "-----------------GOT HERE" << endl;
  //calculate depth
  unsigned int depth = a->calcTableDepth(a);
  if(verbose)
  {
    cout << depth << endl;
  }

  //table html stuff
  string table;
  table.append("<table class =\"table\">\n");
  table.append("<tr>\n");

  table.append("<th>\n");
  table.append("Name");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Type");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Line Number");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Scope");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Scope Depth");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Initialized");
  table.append("</th>\n");

  table.append("<th>\n");
  table.append("Utilized");
  table.append("</th>\n");

  table.append("</tr>\n");
  cout << "-------------BEFORE FOR LOOP" << endl;
  cout << a->rows.size() << endl;
  for(vector<StEntry>::size_type i = 0; i < a->rows.size(); i++)
  {
    cout << "-------------FOR LOOP" << endl;
    table.append("<tr>\n");

    //NAME
    table.append("<td>\n");
    string charToString = "";
    charToString.push_back(a->rows[i].name);
    table.append(charToString);
    table.append("</td>\n");

    //TYPE
    table.append("<td>\n");
    table.append(a->rows[i].type);
    table.append("</td>\n");

    //LINE NUMBER
    table.append("<td>\n");
    table.append(to_string(a->rows[i].lineNum));
    table.append("</td>\n");

    //SCOPE
    table.append("<td>\n");
    table.append(to_string(a->rows[i].scope));
    table.append("</td>\n");

    //SCOPE DEPTH
    table.append("<td>\n");
    table.append(to_string(depth));
    table.append("</td>\n");

    string boolToString;

    //INITIALIZED
    table.append("<td>\n");
    if(a->rows[i].initialized) boolToString = "true";
    else boolToString = "false";
    table.append(boolToString);
    table.append("</td>\n");

    //UTILIZED
    table.append("<td>\n");
    if(a->rows[i].utilized) boolToString = "true";
    else boolToString = "false";
    table.append(boolToString);
    table.append("</td>\n");

    table.append("</tr>\n");
  }
  table.append("</table>");
  //end html

  symbolTableOuput.push_back(table);

  //recursive call
  for (vector<SymbolTable>::size_type i = 0; i < a->children.size(); i++) {
    calcSymbolTableOutput(a->children[i], verbose);
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
         && (tt != "if")
         && (tt != "boolOp")
         && (tt != "assign")
         && (tt != "intOp")
      )
    {
      //put token we want on the heap
      Token* newTerminal = new Token(Semantic::tokens[Semantic::i]);
      //create leaf
      newAST.addChild(newTerminal, false, verbose);


      //varDecl handling
      if(!typeBuffer.empty()) //if typebuffer is set thus next val is that type
      {
        cout << "<><><><><><><><><>We attempting to add var" << endl;
        cout << "<><><><><><><><><>" << typeBuffer << " "
                                     << newTerminal->getData() << " "
                                     << endl;
        //create StEntry to pass on information on from the token
        StEntry t = StEntry(newTerminal->getData()[0],typeBuffer,
        newTerminal->getLine(),curSymbolTable->scope,false);

        //declare the variable in the current scope
        curSymbolTable->declVarTable(t, curSymbolTable);
        typeBuffer.clear(); //clear typebuffer
      }
      if(tt == "type")
      {
        typeBuffer = newTerminal->getData();
      }



    }
    if(verbose)
    {
      cout << "hey we matched: " << tt << endl;
    }
    ++i;  //increment i
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
    //Create new symbolTable - no need to check if it'll fail cause we already
    //ensured it would work in syntax analysis
    //*this passes reference to current object as a parameter
    SymbolTable* newScope = new SymbolTable(curSymbolTable, uniqueScope);
    curSymbolTable = newScope;
    ++uniqueScope; //increment unique scope

    //change current symbol table to newSymbolTable
    curSymbolTable = newScope;

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
    kickST(); //left the current block
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
    resetInExpr(); //reset expression type flags
    return true;
  }
  else //PrintStatement()
  {
    resetInExpr(); //reset expression type flags
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
    resetInExpr(); //reset expression type flags
    return true;
  }
  else //AssignmentStatement()
  {
    resetInExpr(); //reset expression type flags
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
    resetInExpr(); //reset expression type flags
    return true;
  }
  else //VarDecl()
  {
    resetInExpr(); //reset expression type flags
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
    resetInExpr(); //reset expression type flags
    return true;
  }
  else //WhileStatement()
  {
    resetInExpr(); //reset expression type flags
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
    resetInExpr();
    return true;
  }
  else //IfStatement()
  {
    resetInExpr(); //reset expression type flags
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
    resetInExpr(); //reset expression type flags
    return true;
  }
  else //Block()
  {
    resetInExpr(); //reset expression type flags
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
    Token* newBranchBool = new Token("Comp");
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
    Token* newBranchBool = new Token("Comp");
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
  Token* newBranch = new Token("Comp");
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
      inIntExpr = true;
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
     * because an intExpr can turn into an intVal.
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

    inIntExpr = true;
    return true;

  }
  else //digit()
  {
    return false;
  }
}

bool Semantic::IntExpr()
{
  //make sure expressions are made of just int
  if(inBoolExpr || inStringExpr)
  {
    vector<string> errorData;
    if(inBoolExpr) {errorData.push_back("Int and bool are not type compatible");}
    else{errorData.push_back("Int and string are not type compatible");}

    //error for types that are not homogeneous
    Error noHomo(true, Error::semantic, Semantic::tokens[Semantic::i].getLine(),
    Semantic::tokens[Semantic::i].getPos(), errorData, "Type mismatch in expression");
  }
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
        inStringExpr = true;
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
  //make sure expressions are made of just int
  if(inBoolExpr || inIntExpr)
  {
    vector<string> errorData;
    if(inBoolExpr) {errorData.push_back("String and bool are not type compatible");}
      //inIntExpr
    else{errorData.push_back("String and int are not type compatible");}

    //error for types that are not homogeneous
    Error noHomo(true, Error::semantic, Semantic::tokens[Semantic::i].getLine(),
                 Semantic::tokens[Semantic::i].getPos(), errorData, "Type mismatch in expression");
  }
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

//BOOLEANEXPR===========================================================

bool Semantic::BooleanExpr1() //leftParen Expr() boolop() Expr() rightParen
{
  if(term("leftParen"))
  {
    if(Expr())
    {
      if (boolop())
      {
        inBoolExpr = true;
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
    /*
     * Hey future me! This might look confusing at first. Why?!
     * Basically, an extra BoolExpr prints during BooleanExpr
     * because a BoolExpr can turn into a boolVal.
     *
     * This code below gets rid of the extra BooleanExpr by messing
     * with pointers. Good luck!
     */

    //change parent node's grandchild to child
    newAST.curNode->parent->children.back() = newAST.curNode->children[0];
    //change parent of grandchild
    newAST.curNode->children[0]->parent = newAST.curNode->parent;
    //get rid of reference to child in current node
    newAST.curNode->children.empty();
    //make copy of current node so can delete
    Token* copyCur = newAST.curNode;
    //switch to parent
    newAST.curNode = newAST.curNode->parent;
    //delete child
    newAST.deleteNode(copyCur);

    inBoolExpr = true;

    return true;
  }
  else //boolval()
  {
    return false;
  }
}
bool Semantic::BooleanExpr()
{
  //make sure expressions are made of just int
  if(inStringExpr || inIntExpr)
  {
    vector<string> errorData;
    if(inStringExpr) {errorData.push_back("Bool and int are not type compatible");}
      //inIntExpr
    else{errorData.push_back("Bool and int are not type compatible");}

    //error for types that are not homogeneous
    Error noHomo(true, Error::semantic, Semantic::tokens[Semantic::i].getLine(),
                 Semantic::tokens[Semantic::i].getPos(), errorData, "Type mismatch in expression");
  }
  unsigned int save = Semantic::i;
  if( Semantic::i = save, BooleanExpr1())
  {
    kick();
    return true;
  }
  else if( Semantic::i = save, BooleanExpr2())
  {
    //kick();
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
  unsigned int save = Semantic::i;
  if( Semantic::i = save, boolop1())
  {
    return true;
  }
  else
  {
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