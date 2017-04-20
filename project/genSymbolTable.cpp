#include <iostream>

#include "semantic.h"
#include "genSymbolTable.h"

GenSymbolTable::GenSymbolTable(Semantic s, bool v)
  : verbose(v)
{
  tokens = s.tokens;

  //get root tokens
  vector<Token*> rootTokens = s.newAST.treeRoots;

  for(vector<Token*>::size_type i = 0; i < rootTokens.size(); i++)
  {
    //initialize symbol table
    rootSymbolTable = new SymbolTable(nullptr, uniqueScope++);
    curSymbolTable = rootSymbolTable;

    //set curToken for produceST
    curToken = rootTokens[i];
    //call the function to complete the ST generation
    produceST(curToken);
    calcSymbolTableOutput(rootSymbolTable, verbose);

    //free up memory
    deleteAllST(rootSymbolTable);

  }

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
  cout << "PRODUCE ST" << endl;
  //switch to first child of program (block)
  cout << "current tok type" << a->getType() << endl;
  cout << "child tok type" << a->children[0]->getType() << endl;
  if(a->children[0]) //handle nullptr
  {
    curToken = a->children[0];
  }
  //next token will be block
  blockST();
}

bool GenSymbolTable::blockST()
{
  cout << "BLOCK ST" << endl;
  if(curToken->getType() == "Block")
  {
    if(firstBlock)
    {
      //if it's the first block, don't create new symbol table
      firstBlock = false;
    }
    else //not first block
    {
      //create new symbol table
      SymbolTable* newScope = new SymbolTable(curSymbolTable, uniqueScope++);
      newScope->parent = curSymbolTable; //set parent of newScope
      curSymbolTable = newScope; //set current symbolTable to this
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
      else if(ifST());
      else if(printST());
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
  cout << "VARDECL ST" << endl;
  if(curToken->getType() == "VarDecl")
  {
    string varType;
    string varName;
    if(curToken->children[0]) //handle nullptr
    {
      varType = curToken->children[0]->getType();
    }
    if(curToken->children[1]) //handle nullptr
    {
      varName = curToken->children[1]->getData();
    }
    //try to initialize variable
    StEntry variable = StEntry(varName, varType, curToken->getLine(),
                               curToken->getPos(),curSymbolTable->scope,true);
    return true;
  }
  else //not Variable Declaration
  {
    return false;
  }
}

bool GenSymbolTable::assignST()
{
  cout << "ASSIGNSTATEMENT ST" << endl;
  if(curToken->getType() == "AssignStatement")
  {
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
  cout << "IF ST" << endl;
  if(curToken->getType() == "IfStatement")
  {
    //type check the boolean expression
      // -first child will always be the boolexpr branch
    if(curToken->children[0]) //handle nullptr
    {
      boolExprST(curToken->children[0]);
    }
    if(curToken->children[1]) //handle nullptr
    {
      curToken->children[1]; //switch curToken so block can reference
      //handle the block
      curToken->children[1]; //switch curToken so block can reference
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
  cout << "PRINT ST" << endl;
  if(curToken->getType() == "PrintStatement")
  {
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

string GenSymbolTable::exprST(Token* a)
{
  cout << "EXPR ST" << endl;
  string returnType;
  //get type of token
  string tokType;
    //need to determine variable types if applicable
  cout << "Type " << a->getType() << endl;
  if(a->getType() == "char") //variable
  {
    char stringToChar = a->getData()[0]; //get name of var
    //lookup entry, fail if undeclared
    StEntry* lookupVar = curSymbolTable->lookupEntry(stringToChar, curSymbolTable, a->getLine()
      ,a->getPos(),true); //lookup char
    tokType = lookupVar->type;
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
  else if(tokType == "digit" || tokType == "intOp")
  {
    return intExprST(a);
  }
  else if(tokType == "charList")
  {
    return "charList";
  }
}

string GenSymbolTable::boolExprST(Token* a)
{
  cout << "BOOLEXPR ST" << endl;
  string tokType = a->getType();
  string tokVal;
  if(tokType == "==" || tokType == "!=")
  {
    string leftType; //left hand expression type
    string rightType; //right hand expression type
    if(a->children[0]) //handle nullptr
    {
      leftType = intExprST(a->children[0]); //left side recursion
    }
    if(a->children[1]) //handle nullptr
    {
      rightType = intExprST(a->children[1]); //right side recursion
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
  else if(tokType == "charList")
  {
    return "charList";
  }
  else //should not reach this state
  {
    cout << "unreachable state. genSymbolTable.cpp" << endl;
    exit(1);
  }
}

string GenSymbolTable::intExprST(Token* a)
{
  cout << "INTEXPR ST" << endl;
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
  return "charList";
}

//calculate the output for the symbol table
void GenSymbolTable::calcSymbolTableOutput(SymbolTable* a, bool verbose) //depth-first in order
{

  unsigned int depth = a->calcTableDepth(a);

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

  //push output to string vector
  symbolTableOuput.push_back(table);

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
    curSymbolTable = curSymbolTable->parent;
  }
}