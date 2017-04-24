#include <iostream>

#include "semantic.h"
#include "genSymbolTable.h"

using namespace std;

GenSymbolTable::GenSymbolTable(Token* r, bool v)
  : verbose(v), rootToken(r)
{

  //initialize symbol table
  rootSymbolTable = new SymbolTable(nullptr, uniqueScope++);
  curSymbolTable = rootSymbolTable;

  //set curToken for produceST
  curToken = rootToken;

  //call the function to complete the ST generation
  produceST(curToken);

  calcSymbolTableOutput(rootSymbolTable, verbose);

  //dfio search for variables that are declared and not used
  rootSymbolTable->declaredNotUsed(rootSymbolTable);

}

void GenSymbolTable::deleteAllST(SymbolTable *a)
{
  for(vector<SymbolTable*>::size_type i = 0; i < a->children.size(); i++)
  {
    //recursively call for all children
    deleteAllST(a->children[i]);
  }
  delete a;
}

//produce symbol table
void GenSymbolTable::produceST(Token* a)
{
  if (verbose)cout << "PRODUCE ST" << endl;
  //switch to first child of program (block)
  if (verbose)cout << "current tok type" << a->getType() << endl;
  if (verbose)cout << "child tok type" << a->children[0]->getType() << endl;
  if(a->children[0]) //handle nullptr
  {
    curToken = a->children[0];
  }
  //next token will be block
  blockST();
}

bool GenSymbolTable::blockST()
{
  if(curToken->getType() == "Block")
  {
    if (verbose)cout << "BLOCK ST" << endl;
    if(firstBlock)
    {
      //if it's the first block, don't create new symbol table
      firstBlock = false;
    }
    else //not first block
    {
      if (verbose)cout << "Creating new scope" << endl;
      //create new symbol table
      SymbolTable* newScope = new SymbolTable(curSymbolTable, uniqueScope++);
      curSymbolTable->children.push_back(newScope); //add new scope to children of parent
      curSymbolTable = newScope; //set current symbolTable to this
      if (verbose)cout << "switched to scope " << curSymbolTable->scope << endl;
    }

    Token* blockBranch = curToken;
    //loop through children to try other types of statements
    for(vector<Token*>::size_type i = 0; i < blockBranch->children.size(); i++)
    {
      //switch to token
      if(blockBranch->children[i]) //handle nullptr
      {
        curToken = blockBranch->children[i];
      }
      //try different operations
      if(blockST());
      else if(varDeclST());
      else if(assignST());
      else if(printST());
      else if(ifST());
      else if(whileST());
      else //should be unreachable, but throw an error if we reach it
      {
        vector<string> errorData = {curToken->getType()};

        Error newError = Error(true, Error::semantic,curToken->getLine(), curToken->getPos()
        , errorData, "You did the impossible and reached an unreachable state. Congrats! : ");
      }

    }

    kickST(); //move up in symbol table
    return true;
  }
  else //not a block token
  {
    return false;
  }

}

bool GenSymbolTable::varDeclST()
{
  if(curToken->getType() == "VarDecl")
  {
    if (verbose) cout << "VARDECL ST" << endl;
    string varType;
    string varName;
    int varPosition;
    int varLine;
    if(curToken->children[0]) //handle nullptr
    {
      varType = curToken->children[0]->getData();
      varLine = curToken->children[0]->getLine();
      varPosition = curToken->children[0]->getPos();
    }
    if(curToken->children[1]) //handle nullptr
    {
      varName = curToken->children[1]->getData();
    }
    //try to declare variable
    StEntry variable = StEntry(varName, varType, varLine,
                               varPosition,curSymbolTable->scope,false);
    curSymbolTable->declVarTable(variable, curSymbolTable);
    return true;
  }
  else //not Variable Declaration
  {
    return false;
  }
}

bool GenSymbolTable::assignST()
{
  if(curToken->getType() == "AssignStatement")
  {
    if (verbose) cout << "ASSIGNSTATEMENT ST" << endl;
    string varName;
    if(curToken->children[0])
    {
      //first child is always variable name
      varName = curToken->children[0]->getData();
    }
    //let exprST handle type checking of expression
    string exprType = exprST(curToken->children[1]);
    //create template entry
    StEntry assignVar = StEntry(varName,exprType,curToken->children[0]->getLine(), 0,
      curSymbolTable->scope, false);

    //lookup variable to assign
    curSymbolTable->assignVarTable(assignVar);
  }
  else //not assign statement
  {
    return false;
  }
}

bool GenSymbolTable::ifST()
{
  if(curToken->getType() == "IfStatement")
  {
    if (verbose) cout << "IF ST" << endl;
    //type check the boolean expression
      // -first child will always be the boolexpr branch
    if(curToken->children[0]) //handle nullptr
    {
      boolExprST(curToken->children[0]);
    }

    if(curToken->children[1]) //handle nullptr
    {
      //advance token to block
      if (verbose) cout << "In the if. cur token type" << curToken->getType() << endl;
      curToken = curToken->children[1];
    }
    blockST();
    return true;
  }
  else //not an if statement
  {
    return false;
  }
}

bool GenSymbolTable::printST()
{
  if(curToken->getType() == "PrintStatement")
  {
    if (verbose)cout << "PRINT ST" << endl;
    //type check and utilize expression with first child
    if(curToken->children[0]) //handle nullptr
    {
      exprST(curToken->children[0]);
    }

    return true;
  }
  else //not a print statement
  {
    return false;
  }
}

bool GenSymbolTable::whileST()
{
  if(curToken->getType() == "WhileStatement")
  {
    if (verbose) cout << "WHILE ST" << endl;
    //type check the boolean expression
    // -first child will always be the boolexpr branch
    if(curToken->children[0]) //handle nullptr
    {
      boolExprST(curToken->children[0]);
    }
    if(curToken->children[1]) //handle nullptr
    {
      curToken = curToken->children[1];
    }
    blockST();
    return true;
  }
  else //not an if statement
  {
    return false;
  }
}

string GenSymbolTable::exprST(Token* a)
{
  if(verbose) cout << "EXPR ST" << endl;

  string returnType;
  //get type of token
  string tokType;
    //need to determine variable types if applicable
  if(verbose) cout << "Type " << a->getType() << endl;
  if(a->getType() == "char") //variable
  {
    char stringToChar = a->getData()[0]; //get name of var
    if (verbose)cout << "char " << stringToChar << endl;
    //lookup entry, fail if undeclared
    StEntry* lookupVar = curSymbolTable->lookupEntry(stringToChar, curSymbolTable, a->getLine()
      ,a->getPos(),true); //lookup char
    //check that var has been initialized, warning if not
    if(!(lookupVar->initialized)) //not initialized
    {
      vector<string> errorData = {a->getData()};
      Error usedNotInitialized = Error(false, Error::semantic,a->getLine(),a->getPos(), errorData,
                                       "This variable has been declared and used, but not initialized");
    }
    tokType = lookupVar->type;
    if (verbose)cout << tokType << endl;
    lookupVar->utilized = true;
  }
  else //not a variable
  {
    tokType = a->getType();
  }


  //handle type of expression
  if(tokType == "boolVal" || tokType == "==" || tokType == "!=")
  {
    return boolExprST(a);
  }
  //check if intExprST
  else if(tokType == "digit" || tokType == "+" || tokType == "int")
  {
    return intExprST(a);
  }
    //charlist is a raw string, string is the type stored in variable
  else if(tokType == "string")
  {
    return "string";
  }
}

string GenSymbolTable::boolExprST(Token* a)
{
  if (verbose)cout << "BOOLEXPR ST" << endl;

  //handle Comparison error of wrong child
    //we have duplicate comparison tokens and we changed
    //something but didn't get rid of the token so we have to
    //jump over it to get to a "!=" or "=="

  //this shouldn't be needed anymore
  if(a->getType() == "Comp")
  {
    //switch to only child
    a = a->children[0];
  }
  //^^^^^^^^^^^^^
  string tokType = a->getType();
  string tokVal;
  if(tokType == "==" || tokType == "!=")
  {
    string leftType; //left hand expression type
    string rightType; //right hand expression type

    cout << "before the children" << endl;
    if(a->children[0]) //handle nullptr
    {
      cout << "sending child 0 " << a->children[0]->getType() << " " << a->children[0]->getData() << endl;
      leftType = exprST(a->children[0]); //left side recursion
    }
    if(a->children[1]) //handle nullptr
    {
      cout << "sending child 1 " << a->children[1]->getType() << " " << a->children[1]->getData() << endl;
      rightType = exprST(a->children[1]); //right side recursion
    }

    //check for type mismatch
    if(leftType != rightType)
    {
      //error
      vector<string> errorData = {leftType, rightType};
      Error typeMismatch = Error(true, Error::semantic, a->getLine(), a->getPos(), errorData,
        "Type mismatch. These types are not compatible.");
    }
    else //no error
    {
      return "boolVal";
    }
  }
  //boolean
  else if(tokType == "boolVal")
  {
    return "boolVal";
  }
  //integer
  else if(tokType == "digit" || tokType == "intOp")
  {
    //calculate types and
    return intExprST(a);
  }
  //string
  else if(tokType == "string")
  {
    return "string";
  }
  else //should not reach this state
  {
    cout << "unreachable state. genSymbolTable.cpp" << endl;
    cout << tokType << endl;
    cout << tokVal << endl;
    cout << "child of" << endl;
    cout << a->children.back()->getType();
    cout << a->children.back()->getData();
    exit(1);
  }
}

string GenSymbolTable::intExprST(Token* a)
{
  if (verbose)cout << "INTEXPR ST" << endl;
  string tokType = a->getType();
  if(tokType == "+")
  {
    string leftType; //left-hand expression type
    string rightType; //right-hand expression type
    if(a->children[0]) //handle nullptr
    {
      leftType = intExprST(a->children[0]); //left side recursion
    }
    if(a->children[1]) //handle nullptr
    {
      rightType = intExprST(a->children[1]); //right side recursion
    }

    //check for type mismatch
    if(leftType != "digit")
    {
      //error
      vector<string> errorData = {leftType};
      Error typeMismatch = Error(true, Error::semantic, a->getLine(), a->getPos(), errorData,
                                 "Type mismatch. This is not a digit.");
    }
    else if(rightType != "digit")
    {
      //error
      vector<string> errorData = {rightType};
      Error typeMismatch = Error(true, Error::semantic, a->getLine(), a->getPos(), errorData,
                                 "Type mismatch. This is not a digit");
    }
    else //no error
    {
      return "digit";
    }
  }
  else
  {
    return "digit";
  }
}

string GenSymbolTable::stringExprST()
{
  return "string";
}

//calculate the output for the symbol table
void GenSymbolTable::calcSymbolTableOutput(SymbolTable* a, bool verbose) //depth-first in order
{

  unsigned int depth = a->calcTableDepth(a);

  //table html stuff
  string table;

  //label above each table
  string tableLabel;
  //unique scope
  tableLabel.append("<h4>Symbol Table <span class=\"label label-success\">" + to_string(a->scope)
                    + "</span>");
  tableLabel.append("  Scope Depth <span class=\"label label-primary\">" + to_string(depth)
                    + "</span></h4>");

  table.append(tableLabel);


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
  //push output to string vector
  symbolTableOutput.push_back(table);

  //recursive call
  for (vector<SymbolTable>::size_type i = 0; i < a->children.size(); i++) {
    calcSymbolTableOutput(a->children[i], verbose);
  }
}

void GenSymbolTable::kickTok()
{
  if(curToken->parent) //make sure no nullptr
  {
    curToken = curToken->parent;
  }
}

void GenSymbolTable::kickST()
{
  if(curSymbolTable->parent) //make sure no nullptr
  {
    if (verbose)cout << "kicking to parent scope" << endl;
    curSymbolTable = curSymbolTable->parent;
  }
}