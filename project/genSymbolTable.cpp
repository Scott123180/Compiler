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

GenSymbolTable::~GenSymbolTable()
{
  deleteAllSTChildren(rootSymbolTable);
  delete(rootSymbolTable);
}

void GenSymbolTable::deleteAllSTChildren(SymbolTable *a)
{
  while(a->children.size() > 0)
  {
    //call on children
    deleteAllSTChildren(a->children.back());

    SymbolTable* deleteThis = a->children.back();

    //get rid of reference and decrement size
    a->children.pop_back();

    //free memory
    delete(deleteThis);
  }
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

      //switch to children
      curToken = blockBranch->children[i];

      //try different operations
      if(blockST());
      else if(varDeclST());
      else if(assignST());
      else if(printST());
      else if(ifST());
      else if(whileST());

    }

    kickTok(); //kick back token to block above
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
    } else return false; //if vardecl doesn't have any children it can't be a vardecl
    if(curToken->children[1]) //handle nullptr
    {
      varName = curToken->children[1]->getData();
    } else return false; //if vardecl doesn't have a second child, can't be vardecl
    //try to declare variable
    StEntry variable = StEntry(varName, varType, varLine,
                               varPosition,curSymbolTable->scope,false);
    curSymbolTable->declVarTable(variable, curSymbolTable);
    //assign child variable scope (for use in code gen)
    curToken->children[1]->scope = curSymbolTable->scope;
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
      //assign child variable scope (for use in code gen)
      curToken->children[0]->scope = curSymbolTable->scope;
    }
    //let exprST handle type checking of expression
    string exprType = exprST(curToken->children[1]);
    //create template entry
    StEntry assignVar = StEntry(varName,exprType,curToken->children[0]->getLine(), 0,
      curSymbolTable->scope, false);

    //lookup variable to assign
    string lookupType = curSymbolTable->assignVarTable(assignVar);
    //throw error if different type
    if(exprType != lookupType)
    {
      vector<string> errorData = {lookupType, exprType};
      Error assignError = Error(true,Error::semantic, curToken->children[0]->getLine(),
      curToken->children[0]->getPos(), errorData, "Type mismatch in Assign Statment, these types"
                                  " are not compatible: ");
    }
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
      blockST();
    }
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
      //advance token to block
      curToken = curToken->children[1];
      blockST();
    }
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

    //assign variable value of current scope (for use in code gen)
    a->scope = curSymbolTable->scope;
    if(verbose)
    {
      cout << "Lookup var name " << lookupVar->name << endl;
      cout << "Lookup var line:pos " << lookupVar->lineNum << ":" << lookupVar->position << endl;
      cout << "utilized " << lookupVar->utilized << endl;
    }

    return tokType;
  }
  else // not a variable
  {
    if (verbose)
    {
      cout << "NOT A VAR " << "\"" << a->getType() << "\"    \"" << a->getData()
           << "\"" << endl;
    }
    tokType = a->getType();
  }


  //handle type of expression
  if(tokType == "boolVal" || tokType == "boolean" || tokType == "==" || tokType == "!=")
  {
    //check for boolean hell so I can throw error in code gen
    if(tokType == "==" || tokType == "!=")
    {
      string type1 = a->children[0]->getType();
      string type2 = a->children[1]->getType();
      //if children are comparitor operators too
      if(type1 == "!=" || type1 == "==" || type2 == "!=" || type2 == "==")
      {
        //propagate boolean hell error to root symbol table
        SymbolTable* returnToRoot = curSymbolTable;
        while(returnToRoot->parent)
        {
          returnToRoot = returnToRoot->parent;
        }
        returnToRoot->booleanHell = true;
      }
    }

    //fix boolval and boolean mismatch
    if(tokType == "boolVal")
    {
      a->setType("boolean");
    }
    //handle true, false, and var booleans
    if(tokType == "boolean" || tokType == "boolVal")
    {
      return "boolean";
    }
    return boolExprST(a);
  }
  //check if intExprST
  else if(tokType == "digit" || tokType == "+" || tokType == "int")
  {
    //fix digit and make it int
    if(tokType == "digit")
    {
      a->setType("int");
    }
    return intExprST(a);
  }
    //charlist is a raw string, string is the type stored in variable
  else if(tokType == "string" || tokType == "charList")
  {
    //fix charList and make into string
    if(tokType == "charList")
    {
      a->setType("string");
    }
    return "string";
  }
  else
  {
    cout << "WE DUCKED UP IN HANDLING THIS TOKEN" << endl;
    cout << "\"" << a->getType() << "\"" << endl;
    cout << "\"" << a->getData() << "\"" << endl;
    cout << a->getLine() << ":" << a->getPos() << endl;

    exit(1);
  }
}

string GenSymbolTable::boolExprST(Token* a)
{
  if (verbose)
  {
    cout << "BOOLEXPR ST" << endl;
    cout << "===========type " << a->getType() << endl;
    cout << "===========data " << a->getData() << endl;
  }
  string tokType = a->getType();
  string tokVal;
  if(tokType == "==" || tokType == "!=")
  {
    string leftType; //left hand expression type
    string rightType; //right hand expression type

    //cout << "before the children" << endl;
    if(a->children[0]) //handle nullptr
    {
      if(verbose)cout << "sending child 0 " << a->children[0]->getType() << " " << a->children[0]->getData() << endl;
      leftType = exprST(a->children[0]); //left side recursion
    }
    if(a->children[1]) //handle nullptr
    {
      if(verbose)cout << "sending child 1 " << a->children[1]->getType() << " " << a->children[1]->getData() << endl;
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
      return "boolean";
    }
  }
  //boolean
  else if(tokType == "boolVal" || tokType == "boolean")
  {
    //fix boolean and boolVal mismatch
    if(tokType == "boolVal")
    {
      a->setType("boolean");
    }
    return "boolean";
  }
  //integer
  else if(tokType == "digit" || tokType == "intOp" || tokType == "int")
  {
    //fix digit and make it int
    if(tokType == "digit")
    {
      a->setType("int");
    }
    //calculate types and
    return intExprST(a);
  }
  //string
  else if(tokType == "string")
  {
    return "string";
  }
  //variable name
  else if(tokType == "char")
  {
    cout << "~~~~~~~~~ GOT TO THE VAR LOOKUP " << a->getType() << " " << a->getData() << endl;
    return exprST(a);
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

  //fix digit and make it int
  if(tokType == "digit")
  {
    a->setType("int");
  }
  if(tokType == "+")
  {
    string leftType; //left-hand expression type
    string rightType; //right-hand expression type
    if(a->children[0]) //handle nullptr
    {
      if(verbose) cout << "Int expression: left child " << a->children[0]->getType() << endl;
      leftType = exprST(a->children[0]); //left side recursion
      if(verbose) cout << "Int expression: leftType var " << leftType << endl;
    }
    if(a->children[1]) //handle nullptr
    {
      if(verbose) cout << "Int expression: right child " << a->children[1]->getType() << endl;
      rightType = exprST(a->children[1]); //right side recursion
      if(verbose) cout << "Int expression: rightType var " << rightType << endl;
    }

    //check for type mismatch
    if(leftType != "int")
    {
      //error
      vector<string> errorData = {leftType};
      Error typeMismatch = Error(true, Error::semantic, a->children[0]->getLine(),
                                 a->children[0]->getPos(), errorData,
                                 "Type mismatch in integer expression. This is not an integer: ");
    }
    else if(rightType != "int")
    {
      //error
      vector<string> errorData = {rightType};
      Error typeMismatch = Error(true, Error::semantic, a->children[1]->getLine(),
                                 a->children[1]->getPos(), errorData,
                                 "Type mismatch in integer expression. This is not an integer: ");
    }
    else //no error
    {
      return "int";
    }


  }
  else
  {
    //return the type of the token
    return a->getType();
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