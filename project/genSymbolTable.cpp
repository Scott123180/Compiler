#include <iostream>
#include <vector>
#include <string>

#include "genSymbolTable.h"
#include "symbolTable.h"
#include "stEntry.h"
#include "error.h"
#include "genSymbolTable.h"

GenSymbolTable::GenSymbolTable(vector<Token> stream, bool v, unsigned int start)   //v is for verbose
  : verbose(v), i(start)
{

  //remember, if a process fails, we need to delete all of the children
  tokens = stream;

  //initialize symbol table
  rootSymbolTable = new SymbolTable(nullptr, uniqueScope++);
  curSymbolTable = rootSymbolTable;

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
      GenSymbolTable recursiveSemantic(stream, verbose, i);
      //push back ast to original ast
      for(vector<string>::size_type j = 0; j < recursiveSemantic.newAST.tree.size(); j++)
      {
        newAST.tree.push_back(recursiveSemantic.newAST.tree[j]);
      }
      symbolTableOuput.push_back("</br></br></br></br>");
      //push back symbol table to original symbol table
      for(vector<string>::size_type k = 0 ; k < recursiveSemantic.symbolTableOuput.size(); k++)
      {
        symbolTableOuput.push_back(recursiveSemantic.symbolTableOuput[k]);
      }
    }
  }
  else //error
  {
    //TODO: deal with error
  }

}

//kick nodes in the AST
void GenSymbolTable::kick()
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
void GenSymbolTable::kickST()
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
void GenSymbolTable::calcSymbolTableOutput(SymbolTable* a, bool verbose) //depth-first in order
{
  //perform unused identifier checking
  a->declaredNotUsed();

  //check for type mismatches (may be redundant)
  rootSymbolTable->typeMismatch();

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
  for(vector<StEntry>::size_type i = 0; i < a->rows.size(); i++)
  {
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

bool GenSymbolTable::term(string tt) //terminal leaf creation
{
  if (GenSymbolTable::tokens[(GenSymbolTable::i)].getType() == tt)
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
      Token* newTerminal = new Token(GenSymbolTable::tokens[GenSymbolTable::i]);
      //create leaf
      newAST.addChild(newTerminal, false, verbose);

      //charbuffer for id's in assign statement
      if (newTerminal->getType() == "char")
      {
        charBuffer = newTerminal->getData()[0]; //first letter of name

        if(inPrintStatement)
        {
          //change variables in it to utilized
          StEntry* utilize = curSymbolTable->lookupEntry(charBuffer, curSymbolTable);
          curSymbolTable->usedNotDeclared(utilize, newTerminal); //check for error
          curSymbolTable->usedNotInitialized(utilize, newTerminal);
          utilize->utilized = true;
        }
        else //handle other usedNotDeclared
        {
          if(typeBuffer.empty()) //not about to declare variable
          {
            StEntry* check = curSymbolTable->lookupEntry(charBuffer, curSymbolTable);
            curSymbolTable->usedNotDeclared(check, newTerminal); //check for error
          }
        }
      }

      //varDecl handling
      if(!typeBuffer.empty()) //if typebuffer is set thus next val is that type
      {
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
    //boolOp AST handling
    if(tt == "boolOp")
    {
      string boolOperator = GenSymbolTable::tokens[GenSymbolTable::i].getData();
      if(newCompToken) //ensure there is a comp token in buffer
      {
        //this comparison token gets destroyed later because of a duplicate
         //  comparison token problem in the AST, so we will switch to the parent of it
        newCompToken = newCompToken->parent;
        newCompToken->setType(boolOperator); //set type of compToken

        //reset compToken
        newCompToken = nullptr;
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
bool GenSymbolTable::Program1()
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

bool GenSymbolTable::Program()
{
  unsigned int save = GenSymbolTable::i;
  if (GenSymbolTable::i = save, Program1())
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

bool GenSymbolTable::Block1()  //leftBrace, StatementList(), rightBrace
{
  if(term("leftBrace"))
  {
    //programs start with brace so don't want to increment root table
      //(which would make a blank scope)
    if(firstTable)
    {
      //get rid of restriction
      firstTable = false;
    }
    else //no restriction
    {
      //Create new symbolTable - no need to check if it'll fail cause we already
      //ensured it would work in syntax analysis
      //*this passes reference to current object as a parameter
      SymbolTable* newScope = new SymbolTable(curSymbolTable, uniqueScope++); //create new table
      curSymbolTable->children.push_back(newScope); //add table to parent's scope

      //change current symbol table to newSymbolTable
      curSymbolTable = newScope;
    }


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

bool GenSymbolTable::Block()
{
  unsigned int save = GenSymbolTable::i;
  if (GenSymbolTable::i = save, Block1())
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

bool GenSymbolTable::StatementList1() //Statement() StatementList()
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
bool GenSymbolTable::StatementList2() //epsilon
{
  return true;
}

bool GenSymbolTable::StatementList()
{
  unsigned int save = GenSymbolTable::i;
  if(GenSymbolTable::i = save, StatementList1())
  {
    //kick()
    return true;
  }
  else if (GenSymbolTable::i = save, StatementList2())
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

bool GenSymbolTable::Statement1() //PrintStatement()
{
  Token* newBranch = new Token("PrintStatement");
  newAST.addChild(newBranch, true, verbose);
  if(PrintStatement())
  {
    resetInExpr(); //reset expression type flags
    inPrintStatement = false;
    return true;
  }
  else //PrintStatement()
  {
    resetInExpr(); //reset expression type flags
    inPrintStatement = false;
    newAST.deleteNode(newBranch);
    return false;
  }
}
bool GenSymbolTable::Statement2() //AssignStatement()
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
bool GenSymbolTable::Statement3() //VarDecl()
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
bool GenSymbolTable::Statement4() //WhileStatement()
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
bool GenSymbolTable::Statement5() //IfStatement()
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
bool GenSymbolTable::Statement6() //Block()
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

bool GenSymbolTable::Statement()
{
  unsigned int save = GenSymbolTable::i;
  if(GenSymbolTable::i = save, Statement1())
  {

    return true;
  }
  else if (GenSymbolTable::i = save, Statement2())
  {

    return true;
  }
  else if (GenSymbolTable::i = save, Statement3())
  {

    return true;
  }
  else if (GenSymbolTable::i = save, Statement4())
  {

    return true;
  }
  else if (GenSymbolTable::i = save, Statement5())
  {

    return true;
  }
  else if (GenSymbolTable::i = save, Statement6())
  {

    return true;
  }
  else
  {

    return false;
  }
}

//PRINTSTATEMENT========================================================

bool GenSymbolTable::PrintStatement1() //print leftParen Expr() rightParen
{
  if (term("print"))
  {
    //we know we're in a print statement
    inPrintStatement = true;
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

bool GenSymbolTable::PrintStatement()
{
  unsigned int save = GenSymbolTable::i;
  if (GenSymbolTable::i = save, PrintStatement1())
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

bool GenSymbolTable::AssignmentStatement1()  //Id = Expr()
{
  if(Id())
  {
    if(term("assign"))
    {
      //we know it will be an assign statement at this point. let's do operations
      //get name of variable
      //template entry but all we need is line number and name, we lookup the rest later
      StEntry templateEntry = StEntry(charBuffer,"", GenSymbolTable::tokens[GenSymbolTable::i].getLine(),0, false);
      //assign variable and get the type of the variable that we assigned
      string typeExpr = curSymbolTable->assignVarTable(templateEntry);
      if(typeExpr == "int") inIntExpr = true;
      else if(typeExpr == "string") inStringExpr = true;
      else inBoolExpr = true;
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

bool GenSymbolTable::AssignmentStatement()
{
  unsigned int save = GenSymbolTable::i;
  if (GenSymbolTable::i = save, AssignmentStatement1())
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

bool GenSymbolTable::VarDecl1()
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
bool GenSymbolTable::VarDecl()
{
  unsigned int save = GenSymbolTable::i;
  if(GenSymbolTable::i = save, VarDecl1())
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

bool GenSymbolTable::WhileStatement1()  //while BoolExpr() Block()
{
  if(term("while"))
  {
    Token compTime = GenSymbolTable::tokens[GenSymbolTable::i];
    Token* newBranchBool = new Token("Comp");
    newCompToken = newBranchBool;
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

bool GenSymbolTable::WhileStatement()
{
  unsigned int save = GenSymbolTable::i;
  if(GenSymbolTable::i = save, WhileStatement1())
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

bool GenSymbolTable::IfStatement1() //if BooleanExpr() Block()
{
  if(term("if"))
  {
    Token* newBranchBool = new Token("Comp");
    newCompToken = newBranchBool;
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
bool GenSymbolTable::IfStatement()
{
  unsigned int save = GenSymbolTable::i;
  if( GenSymbolTable::i = save, IfStatement1())
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

bool GenSymbolTable::Expr1()  //IntExpr()
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
bool GenSymbolTable::Expr2() //StringExpr()
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
bool GenSymbolTable::Expr3() //BooleanExpr()
{
  Token* newBranch = new Token("Comp");
  newCompToken = newBranch;
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
bool GenSymbolTable::Expr4() //Id()
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

bool GenSymbolTable::Expr()
{
  unsigned int save = GenSymbolTable::i;
  if( GenSymbolTable::i = save, Expr1())
  {
    
    return true;
  }
  else if( GenSymbolTable::i = save, Expr2())
  {
    
    return true;
  }
  else if ( GenSymbolTable::i = save, Expr3())
  {

    return true;
  }
  else if( GenSymbolTable::i = save, Expr4())
  {
    
    return true;
  }
  else
  {
    
    return false;
  }
}

//INTEXPR===============================================================

bool GenSymbolTable::IntExpr1() //digit() intop Expr()
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
bool GenSymbolTable::IntExpr2() //digit()
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

bool GenSymbolTable::IntExpr()
{
  unsigned int save = GenSymbolTable::i;
  if( GenSymbolTable::i = save, IntExpr1())
  {
    //make sure expressions are made of just int
    if(inBoolExpr || inStringExpr)
    {
      vector<string> errorData;
      if(inBoolExpr) {errorData.push_back("Int and bool are not type compatible");}
      else{errorData.push_back("Int and string are not type compatible");}

      //error for types that are not homogeneous
      Error noHomo(true, Error::semantic, GenSymbolTable::tokens[GenSymbolTable::i].getLine(),
                   GenSymbolTable::tokens[GenSymbolTable::i].getPos(), errorData, "Type mismatch in expression");
    }
    kick();
    return true;
  }
  else if( GenSymbolTable::i = save, IntExpr2())
  {
    //make sure expressions are made of just int
    if(inBoolExpr || inStringExpr)
    {
      vector<string> errorData;
      if(inBoolExpr) {errorData.push_back("Int and bool are not type compatible");}
      else{errorData.push_back("Int and string are not type compatible");}

      //error for types that are not homogeneous
      Error noHomo(true, Error::semantic, GenSymbolTable::tokens[GenSymbolTable::i].getLine(),
                   GenSymbolTable::tokens[GenSymbolTable::i].getPos(), errorData, "Type mismatch in expression");
    }
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

bool GenSymbolTable::StringExpr1() //leftQuote CharList() rightQuote
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
bool GenSymbolTable::StringExpr()
{
  unsigned int save = GenSymbolTable::i;
  if( GenSymbolTable::i = save, StringExpr1())
  {
    //make sure expressions are made of just int
    if(inBoolExpr || inIntExpr)
    {
      vector<string> errorData;
      if(inBoolExpr) {errorData.push_back("String and bool are not type compatible");}
        //inIntExpr
      else{errorData.push_back("String and int are not type compatible");}

      //error for types that are not homogeneous
      Error noHomo(true, Error::semantic, GenSymbolTable::tokens[GenSymbolTable::i].getLine(),
                   GenSymbolTable::tokens[GenSymbolTable::i].getPos(), errorData, "Type mismatch in expression");
    }
    return true;
  }
  else
  {
    return false;
  }
}

//BOOLEANEXPR===========================================================

bool GenSymbolTable::BooleanExpr1() //leftParen Expr() boolop() Expr() rightParen
{
  if(term("leftParen"))
  {
    if(Expr())
    {
      if (boolop())
      {
        //use as a marker for the right hand expression type checking
        unsigned int secondBackToken = GenSymbolTable::i;
        string type;
        //check previous token to determine expression type
        unsigned int backToken = GenSymbolTable::i;
        bool found = false;
        while(!found)
        {
          --backToken;
          Token comparisonToken = GenSymbolTable::tokens[backToken];
          if(comparisonToken.getType() == "char") //lookup variable types
          {
            char stringToChar = comparisonToken.getData()[0];
            StEntry* comparisonEntry = curSymbolTable->lookupEntry(stringToChar, curSymbolTable);
            type = comparisonEntry->type;
            //check if been assigned variable
            if(type == "int" || type == "string" || type == "boolean")
            {
              if(!comparisonEntry->hasIntBeenSet)
              {
                vector<string> errorData = {type, comparisonToken.getData()};
                Error usedNotDeclared = Error(true, Error::semantic, comparisonToken.getLine(),
                                              comparisonToken.getPos(), errorData, "Undeclared Identifier Used ");
              }
            }
            if(type == "int") inIntExpr = true;
            else if (type == "string") inStringExpr = true;
              //different name for token values and symboltable entries
            else if (type == "boolean") inBoolExpr = true;

            found = true;
          }
          else if(comparisonToken.getType() == "digit")
          {
            type = "int";
            inIntExpr = true;
            found = true;
          }
          else if(comparisonToken.getType() == "charList")
          {
            type = "string";
            inStringExpr = true;
            found = true;
          }
          else if(comparisonToken.getType() == "boolVal")
          {
            type = "boolean";
            inBoolExpr = true;
            found = true;
          }
          else
          {
            //onto next token
          }
        }
        if(Expr())
        {
          if(term("rightParen"))
          {
            bool foundRight = false;
            backToken = GenSymbolTable::i; //reset back token to cur token
            string typeRight;
            while(!foundRight || (secondBackToken != backToken))
            {
              --backToken;
              Token comparisonTokenRight = GenSymbolTable::tokens[backToken];
              if(comparisonTokenRight.getType() == "char") //lookup variable types
              {
                char stringToChar = comparisonTokenRight.getData()[0];
                StEntry* comparisonEntryR = curSymbolTable->lookupEntry(stringToChar, curSymbolTable);
                curSymbolTable->usedNotDeclared(comparisonEntryR, &GenSymbolTable::tokens[backToken]);
                typeRight = comparisonEntryR->type;
                //check if been assigned variable
                if(type == "int" || type == "string" || type == "boolean")
                {
                  if(!comparisonEntryR->hasIntBeenSet)
                  {
                    vector<string> errorData = {type, comparisonTokenRight.getData()};
                    Error usedNotDeclared = Error(true, Error::semantic, comparisonTokenRight.getLine(),
                                                  comparisonTokenRight.getPos(), errorData, "Undeclared Identifier Used ");
                  }
                }
                if(typeRight == "int") inIntExpr = true;
                else if (typeRight == "string") inStringExpr = true;
                  //different name for token values and symboltable entries
                else if (typeRight == "boolean") inBoolExpr = true;

                foundRight = true;
              }
              else if(comparisonTokenRight.getType() == "digit")
              {
                typeRight = "int";
                inIntExpr = true;
                foundRight = true;
              }
              else if(comparisonTokenRight.getType() == "charList")
              {
                typeRight = "string";
                inStringExpr = true;
                foundRight = true;
              }
              else if(comparisonTokenRight.getType() == "boolVal")
              {
                typeRight = "boolean";
                inBoolExpr = true;
                foundRight = true;
              }
              else
              {
                //onto next token
              }
            }
            if(typeRight != type)//error type mismatch
            {
              vector<string> errorData = {type, typeRight};
              int lineNum = GenSymbolTable::tokens[GenSymbolTable::i].getLine();
              int position = GenSymbolTable::tokens[GenSymbolTable::i].getPos();
              Error typeMismatch = Error(true,Error::semantic,lineNum,position, errorData,
                "Type mismatch in comparison");
            }

            //end of bool expr
            resetInExpr();
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
bool GenSymbolTable::BooleanExpr2() //boolval()
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
bool GenSymbolTable::BooleanExpr()
{
  unsigned int save = GenSymbolTable::i;
  if( GenSymbolTable::i = save, BooleanExpr1())
  {
    //make sure expressions are made of just boolean
    if(inStringExpr || inIntExpr)
    {
      vector<string> errorData;
      if(inStringExpr) {errorData.push_back("Bool and string are not type compatible");}
        //inIntExpr
      else{errorData.push_back("Bool and int are not type compatible");}

      //error for types that are not homogeneous
      Error noHomo(true, Error::semantic, GenSymbolTable::tokens[GenSymbolTable::i].getLine(),
                   GenSymbolTable::tokens[GenSymbolTable::i].getPos(), errorData, "Type mismatch in expression");
    }
    kick();
    return true;
  }
  else if( GenSymbolTable::i = save, BooleanExpr2())
  {
    //make sure expressions are made of just boolean
    if(inStringExpr || inIntExpr)
    {
      vector<string> errorData;
      if(inStringExpr) {errorData.push_back("Bool and string are not type compatible");}
        //inIntExpr
      else{errorData.push_back("Bool and int are not type compatible");}

      //error for types that are not homogeneous
      Error noHomo(true, Error::semantic, GenSymbolTable::tokens[GenSymbolTable::i].getLine(),
                   GenSymbolTable::tokens[GenSymbolTable::i].getPos(), errorData, "Type mismatch in expression");
    }
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

bool GenSymbolTable::Id1() //Char()
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
bool GenSymbolTable::Id()
{
  unsigned int save = GenSymbolTable::i; if ( GenSymbolTable::i = save, Id1())
  {
    
    return true;
  }
  else
  {
    
    return false;
  }
}

//CHARLIST==============================================================

bool GenSymbolTable::CharList1() //Char() CharList()
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
bool GenSymbolTable::CharList2() //epsilon
{
  return true;
}
bool GenSymbolTable::CharList()
{
  unsigned int save = GenSymbolTable::i;
  if ( GenSymbolTable::i = save, CharList1())
  {
    
    return true;
  }
  else if ( GenSymbolTable::i = save, CharList2())
  {
    
    return true;
  }
  else
  {
    
    return false;
  }
}

//TYPE==================================================================

bool GenSymbolTable::type1() //type
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
bool GenSymbolTable::type()
{
  unsigned int save = GenSymbolTable::i;
  if ( GenSymbolTable::i = save, type1())
  {
    
    return true;
  }
  else
  {
    
    return false;
  }
}

//CHAR==================================================================

bool GenSymbolTable::Char1()
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
bool GenSymbolTable::Char()
{
  unsigned int save = GenSymbolTable::i;
  if ( GenSymbolTable::i = save, Char1())
  {
    
    return true;
  }
  else
  {
    
    return false;
  }
}

//SPACE=================================================================

bool GenSymbolTable::space1()
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
bool GenSymbolTable::space()
{
  unsigned int save = GenSymbolTable::i;
  if ( GenSymbolTable::i = save, space1())
  {
    
    return true;
  }
  else
  {
    
    return false;
  }
}

//DIGIT=================================================================

bool GenSymbolTable::digit1()
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
bool GenSymbolTable::digit()
{
  unsigned int save = GenSymbolTable::i;
  if( GenSymbolTable::i = save, digit1())
  {

    return true;
  }
  else
  {

    return false;
  }
}

//BOOLOP================================================================

bool GenSymbolTable::boolop1()
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
bool GenSymbolTable::boolop()
{
  unsigned int save = GenSymbolTable::i;
  if( GenSymbolTable::i = save, boolop1())
  {
    return true;
  }
  else
  {
    return false;
  }
}

//BOOLVAL===============================================================

bool GenSymbolTable::boolval1()
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
bool GenSymbolTable::boolval()
{
  unsigned int save = GenSymbolTable::i;
  if( GenSymbolTable::i = save, boolval1())
  {
    return true;
  }
  else
  {
    return false;
  }
}

//INTOP=================================================================

bool GenSymbolTable::intop1() {
  if(term("intOp"))
  {
    return true;
  }
  else //intop
  {
    return false;
  }
}
bool GenSymbolTable::intop()
{
  unsigned int save = GenSymbolTable::i;
  if( GenSymbolTable::i = save, intop1())
  {
    return true;
  }
  else
  {
    return false;
  }
}